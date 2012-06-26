package main;

import org.apache.log4j.Logger;
import org.apache.log4j.PropertyConfigurator;

class Main {

    private static final Logger log_ = Logger.getLogger(Main.class);

    private static void parseParameters(final Agent.Configuration config, final String[] args) throws Exception {
        config.port = 8080;
        config.isDebug = false;
        config.ssl = 0;
        for (int i = 0; i < args.length; ++i) {
            final String it = args[i];
            if (it.equals("--port")) {
                if (i + 1 == args.length)
                    throw new Exception("Missing --port parameter");
                config.port = Integer.parseInt(args[++i]);
            } else if (it.equals("--ssl")) {
                if (i + 1 == args.length)
                    throw new Exception("Missing --ssl parameter");
                config.ssl = Integer.parseInt(args[++i]);
            } else if (it.equals("--ssl_store")) {
                if (i + 1 == args.length)
                    throw new Exception("Missing --ssl_store parameter");
                config.store = args[++i];
            } else if (it.equals("--ssl_type")) {
                if (i + 1 == args.length)
                    throw new Exception("Missing --ssl_type parameter");
                config.type = args[++i];
            } else if (it.equals("--ssl_password")) {
                if (i + 1 == args.length)
                    throw new Exception("Missing --ssl_password parameter");
                config.password = args[++i];
            } else if (it.equals("--debug")) {
                config.isDebug = true;
            } else {
                throw new Exception("Unrecognized parameter " + it);
            }
        }
    }

    private static void printParameters(final Agent.Configuration config) {
        log_.info("port:  " + config.port);
        log_.info("debug: " + (config.isDebug ? "true" : "false"));
        if (config.ssl == 0)
            return;
        log_.info("ssl: " + config.ssl);
        log_.info("store: " + config.store);
        log_.info("type: " + config.type);
    }

    public static void main(final String[] args) {

        PropertyConfigurator.configure("log4j.properties");
        log_.info("Sword Proxy - copyright Masa Group 2012");

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
