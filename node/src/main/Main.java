package main;

import org.apache.log4j.BasicConfigurator;
import org.apache.log4j.Logger;

class Main {

    private static final Logger logger_ = Logger.getLogger(Main.class);

    public static void main(final String[] args) {

        BasicConfigurator.configure();
        logger_.info("Sword Node - copyright Masa Group 2012");

    }
}
