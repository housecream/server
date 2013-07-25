/*global module:false, require:false*/

/* default config can be overwritten by user specific settings in userconfig.json */
var defaultconfig = {
    "livereloadport": 5000
};

/* live reload snippet */
var path = require('path');
var lrSnippet = require('grunt-contrib-livereload/lib/utils').livereloadSnippet;

var folderMount = function folderMount(connect, point) {
    return connect.static(path.resolve(point));
};

module.exports = function (grunt) {
    'use strict';

    /**
     * Load external grunt helpers & tasks
     */
    grunt.loadNpmTasks('grunt-contrib-connect');
    grunt.loadNpmTasks('grunt-contrib-livereload');
    grunt.loadNpmTasks('grunt-contrib-stylus');
    grunt.loadNpmTasks('grunt-karma');
    grunt.loadNpmTasks('grunt-regarde');

    /**
     * Project configuration.
     */
    grunt.initConfig({
        // Metadata.
        pkg: grunt.file.readJSON('package.json'),
        config: grunt.file.exists('userconfig.json') ? grunt.util._.extend(defaultconfig, grunt.file.readJSON('userconfig.json')) : defaultconfig,
        dirs: {
            source: 'public/',
            server: 'server/',
            production: 'build/',
            docs: 'docs/'
        },
        now: new Date(),
        banner: '/*! <%= pkg.title || pkg.name %> - v<%= pkg.version %> - ' +
            '<%= grunt.template.today("yyyy-mm-dd") %>\n' +
            '<%= pkg.homepage ? "* " + pkg.homepage + "\\n" : "" %>' +
            '* Copyright (c) <%= grunt.template.today("yyyy") %> <%= pkg.author.name %>\n' +
            '*\n* DO NOT EDIT THIS FILE, IT WILL BE OVERWRITTEN ON NEXT BUILD\n*/\n',

        // tasks

        // compile stylus templates into css
        stylus: {
            compile: {
                options: {

                },
                files: {
                    'public/styles/styles.compiled.css': ['public/styles/*.styl']
                }
            }
        },

        // karma run unit-tests && end to end testing
        karma: {
            unitserver: {
                background: true,
                configFile: 'config/karma.conf.js'
            },
            unit: {
                configFile: 'config/karma.conf.js'
            },
            e2e: {
                configFile: 'config/karma-e2e.conf.js'
            },
            continuous: {
                configFile: 'config/karma.conf.js',
                singleRun: true
            }
        },

        // livereload: reloading the browser whenever files are changed
        connect: {
            livereload: {
                options: {
                    port: '<%= config.livereloadport %>',
                    middleware: function (connect, options) {
                        // for some reason, this argument can't use <%= dirs.source %>
                        // it needs to be set to build/ directly
                        return [lrSnippet, folderMount(connect, 'public/')];
                    }
                }
            }
        },

        // regarde watches files and executes tasks when a file changes
        regarde: {
            stylus: {
                files: '<%= dirs.source %>styles/**/*.styl',
                tasks: ['stylus', 'livereload']
            },
            javascript: {
                files: '<%= dirs.source %>**/*.js',
                tasks: ['livereload','karma:continuous:run']
            },
            html: {
                files: '<%= dirs.source %>**/*.html',
                tasks: ['livereload']
            }
        }
    });

    /**
     * Define tasks groups
     */
    grunt.registerTask('dev', [
        'dev-build',
        'dev-test',
        'dev-load'
    ]);

    grunt.registerTask('dev-build', [
        'stylus'
    ]);

    grunt.registerTask('dev-test', [
        'karma:unitserver'
    ]);

    grunt.registerTask('dev-load', [
        'livereload-start',
        'connect',
        'regarde'
    ]);

    grunt.registerTask('test', [
        'dev-build',
        'karma:e2e',
        'karma:unit'
    ]);

    // Default
    grunt.registerTask('default', ['dev']);
};