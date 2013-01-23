package net.awired.housecream.server.core.application;

import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.net.URL;
import java.util.Enumeration;
import java.util.jar.Manifest;
import javax.naming.Context;
import javax.naming.InitialContext;
import javax.naming.NamingException;

public enum Housecream {
    INSTANCE;

    public static final String HOUSECREAM_NAME = "Housecream";
    public static final String HOME_KEY = "HOUSECREAM_HOME";

    private static final String LOG_CONF_FILE_PATH_KEY = "logback.configurationFile";
    private static final String LOG_CONF_FILE_NAME = "logback.xml";

    private static final String VERSION_MANIFEST_KEY = "HousecreamVersion";
    private static final String VERSION_UNKNOWN = "Unknow Version";

    private File home;
    private String version;

    private Housecream() {
        home = findHomeDir();
        version = findVersion(null);

        System.setProperty(HOME_KEY, home.getAbsolutePath());
        if (!home.exists()) {
            home.mkdirs();
        }

        File logbackConf = new File(home, LOG_CONF_FILE_NAME);
        if (!logbackConf.exists()) {
            copyLogbackFile();
        }
        if (System.getProperty(LOG_CONF_FILE_PATH_KEY) == null) {
            System.setProperty(LOG_CONF_FILE_PATH_KEY, logbackConf.getAbsolutePath());
        }

    }

    public void updateVersion(InputStream manifest) {
        if (manifest == null) {
            return;
        }
        version = findVersion(manifest);
    }

    public void printInfo() {
        System.out.println("Housecream version : " + version);
        System.out.println("Housecream home : " + home);
    }

    public File getHome() {
        return home;
    }

    public String getVersion() {
        return version;
    }

    //////////////////////////////////

    private File findHomeDir() {
        try {
            InitialContext iniCtxt = new InitialContext();
            Context env = (Context) iniCtxt.lookup("java:comp/env");
            String value = (String) env.lookup(HOME_KEY);
            if (value != null && value.trim().length() > 0) {
                return new File(value.trim());
            }

            value = (String) iniCtxt.lookup(HOME_KEY);
            if (value != null && value.trim().length() > 0) {
                return new File(value.trim());
            }
        } catch (NamingException e) {
        }

        String sysProp = System.getProperty(HOME_KEY);
        if (sysProp != null) {
            return new File(sysProp.trim());
        }

        try {
            String env = System.getenv(HOME_KEY);
            if (env != null) {
                return new File(env.trim());
            }
        } catch (Throwable _) {
        }
        return new File(System.getProperty("user.home") + "/." + HOUSECREAM_NAME.toLowerCase());
    }

    private void copyLogbackFile() {
        InputStream from = null;
        OutputStream to = null;
        try {
            from = getClass().getResourceAsStream("/housecream-logback.xml");
            to = new FileOutputStream(new File(home, LOG_CONF_FILE_NAME));
            byte[] buf = new byte[0x800];
            int bytesRead = from.read(buf);
            while (bytesRead > -1) {
                to.write(buf, 0, bytesRead);
                bytesRead = from.read(buf);
            }
        } catch (IOException e) {
            System.err.println("Cannot write logback configuration file to home folder : " + home);
            e.printStackTrace();
        } finally {
            if (from != null) {
                try {
                    from.close();
                } catch (IOException e) {
                }
            }
            if (to != null) {
                try {
                    to.close();
                } catch (IOException e) {
                }
            }
        }
    }

    private String findVersion(InputStream manifestIn) {
        // runnnable war
        try {
            Enumeration<URL> manifests = getClass().getClassLoader().getResources("META-INF/MANIFEST.MF");
            while (manifests.hasMoreElements()) {
                URL res = manifests.nextElement();
                Manifest manifest = new Manifest(res.openStream());
                String versionManifest = manifest.getMainAttributes().getValue(VERSION_MANIFEST_KEY);
                if (versionManifest != null) {
                    return versionManifest;
                }
            }
        } catch (IOException e) {
        }

        if (manifestIn != null) {
            // tomcat like
            try {
                Manifest manifest = new Manifest(manifestIn);
                String versionManifest = manifest.getMainAttributes().getValue(VERSION_MANIFEST_KEY);
                if (versionManifest != null) {
                    return versionManifest;
                }
            } catch (IOException e) {
            }
        }
        return VERSION_UNKNOWN;
    }

}