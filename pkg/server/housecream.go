package server

import (
	"fmt"

	"context"

	"reflect"

	"time"

	"io/ioutil"

	"github.com/n0rad/go-erlog/logs"
	"github.com/n0rad/housecream/pkg/channels"
	"github.com/n0rad/housecream/pkg/channels/openweathermap"
	"github.com/n0rad/housecream/pkg/prometheus"
	"github.com/prometheus/common/model"
	"github.com/prometheus/prometheus/promql"
)

type Rule struct {
}

type Housecream struct {
	Port   int
	Points []channels.Link
	//Rules  []Rule

	buildVersion BuildVersion
	//HomeFolder string
	shutdownCtx  context.Context
	shutdownFunc context.CancelFunc
}

func NewHousecream(buildVersion BuildVersion) *Housecream {
	return &Housecream{
		Port:         4242,
		buildVersion: buildVersion,
	}
}

const prom = `global:
  evaluation_interval: 1m
  scrape_interval:     1m
  scrape_timeout:      10s


scrape_configs:
- job_name: 'prometheus'`

func (h *Housecream) Start(home HomeFolder) {

	server := NewGrafanaServer(&home)
	go server.Start()

	promql.StalenessDelta = 30 * time.Minute

	// TODO
	logs.SetLevel(logs.DEBUG)

	if err := ioutil.WriteFile(home.Path+"/prometheus.yml", []byte(prom), 0644); err != nil {
		logs.WithE(err).Fatal("Failed to write prometheus.yml")
	}

	prometheus.ConfigFile(home.Path + "/prometheus.yml")
	prometheus.Parse([]string{"--storage.local.path=" + home.Path + "/prometheus"})

	localStorage := prometheus.LocalStorage()

	go prometheus.Main(localStorage)

	//h.shutdownCtx, h.shutdownFunc = context.WithCancel(context.Background())
	home.LoadConfig(h)
	if err := home.SaveVersion(&h.buildVersion); err != nil {
		logs.WithE(err).Fatal("Cannot save version")
	}

	eventsIn := make(chan channels.Event)
	go func() {
		for {
			select {
			case event := <-eventsIn:
				logs.WithField("event", event).Debug("Receiving an event")

				//value := reflect.ValueOf(event.Value).Float()
				//fmt.Printf("%T %d", v, v)

				//value, ok := event.Value.(float64)
				//if ok {

				var value float64
				rv := reflect.ValueOf(event.Value)
				switch rv.Kind() {
				case reflect.Int, reflect.Int8, reflect.Int16, reflect.Int32, reflect.Int64:
					value = (float64(rv.Int()))
				case reflect.Uint, reflect.Uintptr, reflect.Uint8, reflect.Uint16, reflect.Uint32, reflect.Uint64:
					value = (float64(rv.Uint()))
				case reflect.Float32, reflect.Float64:
					value = rv.Float()
					//default:
					//	return "fmtNumber/fmtDecimal expects a numerical value to format"
				}

				sample := &model.Sample{
					Metric: model.Metric{
						model.MetricNameLabel: model.LabelValue("housecream_link_" + event.LinkName + "_" + event.PointName),
						"link":                model.LabelValue(event.LinkName),
					},
					Timestamp: model.TimeFromUnixNano(event.Time.UnixNano()),
					Value:     model.SampleValue(value),
				}
				localStorage.Append(sample)
				logs.WithField("point", event.PointName).Debug("append metric")
				//}
			}
		}
	}()

	owm := openweathermap.OpenWeatherMapLink{
		CommonLink: channels.CommonLink{
			Name:    "lardy",
			Channel: "openweathermap",
		},
		ApiKey:    "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX",
		Latitude:  48.52,
		Longitude: 2.26,
	}

	h.Points = append(h.Points, &owm)
	owm.CommonLink.Init()
	owm.Init()

	lifeCycle := NewLinkLifecycle(&owm)
	lifeCycle.Start(eventsIn)

	//cc := cron.CronLink{
	//	Rule:       "@every 1s",
	//	CommonLink: channels.CommonLink{Channel: "cron"},
	//}
	//h.Points = append(h.Points, &cc)
	//cc.Init()
	//lifecycle := NewPointLifecycle(&cc)
	//go lifecycle.Start(eventsIn)
	//time.AfterFunc(40*time.Second, func() {
	//	lifecycle.Stop()
	//})

	//c := slack.SlackLink{
	//	ApiKey: "",
	//}
	//h.Points = append(h.Points, &c)
	//
	//if err := c.CommonLink.Init(); err != nil {
	//	logs.WithE(err).Warn("failed to init slack")
	//}
	//if err := c.Init(); err != nil {
	//	logs.WithE(err).Warn("failed to init slack")
	//}
	//background := context.Background()
	//
	//lifecycle2 := NewPointLifecycle(&c)
	//go lifecycle2.Start(eventsIn)

	//time.AfterFunc(30+time.Second, func() {
	//	lifecycle2.Stop()
	//})

	////go c.Watch(background, eventsIn)
	//if err := c.Trigger(background, channels.Action{}); err != nil {
	//	logs.WithE(err).Warn("failed to trigger")
	//}

	home.SaveConfig(h)

	for _, channel := range channels.ChannelByIds {
		fmt.Println("->", channel)
	}
	//housecream.StartEngine()
}

func (h Housecream) Stop() {
	h.shutdownFunc()
}
