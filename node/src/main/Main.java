package main;

import org.apache.log4j.Logger;
import org.apache.log4j.PropertyConfigurator;
import org.eclipse.jetty.server.Server;

import web.Handler;

class Main {

    private static final Logger log_ = Logger.getLogger(Main.class);

    public static void main(final String[] args) {

        PropertyConfigurator.configure("log4j.properties");
        log_.info("Sword Node - copyright Masa Group 2012");

        final Server server = new Server(8080);
        try {
            server.setHandler(new Handler("e:/cloud_hg/node/www"));
            server.start();
        } catch (final Exception e) {
            log_.error(e.toString());
        }
    }
}
