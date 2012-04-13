package main;

import java.io.File;

import org.apache.log4j.Logger;
import org.apache.log4j.PropertyConfigurator;

import web.Agent;

class Main {

    private static final Logger log_ = Logger.getLogger(Main.class);

    private static void parseParameters(final Agent.Configuration config, final String[] args) throws Exception {
        config.root = "www";
        config.type = "cluster";
        config.name = "";
        config.port = 8081;
        config.proxy = 8080;
        config.isDebug = false;
        for (int i = 0; i < args.length; ++i) {
            final String it = args[i];
            if (it.equals("--root") || it.equals("-r")) {
                if (i + 1 == args.length)
                    throw new Exception("Missing --www parameter");
                config.root = args[++i];
            } else if (it.equals("--port") || it.equals("-p")) {
                if (i + 1 == args.length)
                    throw new Exception("Missing --port parameter");
                config.port = Integer.parseInt(args[++i]);
            } else if (it.equals("--proxy")) {
                if (i + 1 == args.length)
                    throw new Exception("Missing --proxy parameter");
                config.proxy = Integer.parseInt(args[++i]);
            } else if (it.equals("--name")) {
                if (i + 1 == args.length)
                    throw new Exception("Missing --name parameter");
                config.name = args[++i];
            } else if (it.equals("--uuid") || it.equals("-u")) {
                if (i + 1 == args.length)
                    throw new Exception("Missing --uuid parameter");
                config.uuid = args[++i];
            } else if (it.equals("--debug") || it.equals("-d")) {
                config.isDebug = true;
            } else if (it.equals("--type") || it.equals("-t")) {
                if (i + 1 == args.length)
                    throw new Exception("Missing --type parameter");
                config.type = args[++i];
            } else {
                throw new Exception("Unrecognized parameter " + it);
            }
        }
        final File dir = new File(config.root);
        if (!dir.isDirectory())
            throw new Exception(dir.getAbsolutePath() + " is not a directory");
        config.root = dir.getAbsolutePath();
        if (config.uuid.isEmpty())
            throw new Exception("Missing --uuid parameter");
    }

    private static void printParameters(final Agent.Configuration config) {
        log_.info("uuid:  " + config.uuid);
        log_.info("root:  " + config.root);
        log_.info("port:  " + config.port);
        log_.info("proxy: " + config.proxy);
        log_.info("type:  " + config.type);
        log_.info("debug: " + (config.isDebug ? "true" : "false"));
    }

    public static void main(final String[] args) {

        PropertyConfigurator.configure("log4j.properties");
        log_.info("Sword Node - copyright Masa Group 2012");

        final Agent.Configuration config = new Agent.Configuration();
        try {
            parseParameters(config, args);
        } catch (final Exception err) {
            log_.error(err.toString());
            log_.error("Unable to parse command-line parameters");
            return;
        }
        printParameters(config);

        try {
            final Agent agent = new Agent(config);
            agent.exec();
        } catch (final Exception err) {
            log_.error(err.toString());
            log_.error("Internal agent error");
        }
    }
}
