package main;

import org.apache.log4j.Logger;
import org.apache.log4j.PropertyConfigurator;

import web.Agent;

class Main {

    private static final Logger log_ = Logger.getLogger(Main.class);

    public static void main(final String[] args) {

        PropertyConfigurator.configure("log4j.properties");
        log_.info("Sword Node - copyright Masa Group 2012");
        try {
            final Agent agent = new Agent("e:/cloud_hg/node/www", 8080, 15000, true);
            agent.exec();
        } catch (final Exception err) {
            log_.error("Unexpected error: " + err.toString());
        }
    }
}
