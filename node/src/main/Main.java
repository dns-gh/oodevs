package main;

import org.apache.log4j.BasicConfigurator;
import org.apache.log4j.Logger;
import org.eclipse.jetty.server.Server;

import web.Handler;

class Main {

    private static final Logger log_ = Logger.getLogger(Main.class);

    public static void main(final String[] args) {

        BasicConfigurator.configure();
        log_.info("Sword Node - copyright Masa Group 2012");

        final Server server = new Server(8080);
        server.setHandler(new Handler());
        try {
            server.start();
        } catch (final Exception e) {
            log_.error(e.toString());
        }
    }
}
