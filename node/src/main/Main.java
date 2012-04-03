package main;

import java.io.File;

import org.apache.log4j.Logger;
import org.apache.log4j.PropertyConfigurator;

import web.Agent;

class MainConfig {
    public String root;
    public int port;
    public int host;
    public boolean isDebug;
};

class Main {

    private static final Logger log_ = Logger.getLogger(Main.class);

    private static MainConfig parseParameters(final String[] args) throws Exception {
        final MainConfig reply = new MainConfig();
        reply.root = "www";
        reply.port = 8080;
        reply.host = 15000;
        reply.isDebug = false;
        for (int i = 0; i < args.length; ++i) {
            final String it = args[i];
            if (it.equals("--root") || it.equals("-r")) {
                if (i + 1 == args.length)
                    throw new Exception("Missing --www parameter");
                reply.root = args[++i];
            } else if (it.equals("--port") || it.equals("-p")) {
                if (i + 1 == args.length)
                    throw new Exception("Missing --port parameter");
                reply.port = Integer.parseInt(args[++i]);
            } else if (it.equals("--host") || it.equals("-h")) {
                if (i + 1 == args.length)
                    throw new Exception("Missing --host parameter");
                reply.host = Integer.parseInt(args[++i]);
            } else if (it.equals("--debug") || it.equals("-d")) {
                reply.isDebug = true;
            } else {
                throw new Exception("Unrecognized parameter " + it);
            }
        }
        final File dir = new File(reply.root);
        if (!dir.isDirectory())
            throw new Exception(dir.getAbsolutePath() + " is not a directory");
        reply.root = dir.getAbsolutePath();
        return reply;
    }

    private static void printParameters(final MainConfig config) {
        log_.info("root:  " + config.root);
        log_.info("port:  " + config.port);
        log_.info("host:  " + config.host);
        log_.info("debug: " + (config.isDebug ? "true" : "false"));
    }

    public static void main(final String[] args) {

        PropertyConfigurator.configure("log4j.properties");
        log_.info("Sword Node - copyright Masa Group 2012");

        MainConfig config = null;
        try {
            config = parseParameters(args);
        } catch (final Exception err) {
            log_.error(err.toString());
            log_.error("Unable to parse command-line parameters");
            return;
        }
        printParameters(config);

        try {
            final Agent agent = new Agent(config.root, config.port, config.host, config.isDebug);
            agent.exec();
        } catch (final Exception err) {
            log_.error(err.toString());
            log_.error("Internal agent error");
        }
    }
}
