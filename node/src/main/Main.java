package main;

import java.io.IOException;

import org.apache.log4j.BasicConfigurator;
import org.apache.log4j.Logger;

import process.ProcessHandler;

class Main {

    private static final Logger logger_ = Logger.getLogger(Main.class);

    public static void main(final String[] args) {

        BasicConfigurator.configure();
        logger_.info("Sword Node - copyright Masa Group 2012");

        testProcessHandler();
    }

    private static void testProcessHandler() {
        final String app_dir = "E:/sword5_hg/out/vc100/Debug/applications/simulation_app";
        final ProcessHandler handler = new ProcessHandler("sword_simulation",
                app_dir + "/simulation_app_d.exe", "--root-dir=../../data",
                "--exercise=worldwide/Egypt", "--session=default");
        try {
            final String run_dir = "e:/sword5_hg/run/vc100";
            handler.start(run_dir);
        } catch (final IOException e) {
            logger_.error(e);
        }

        final int reply = handler.join(10 * 1000);
        System.out.println("exit code: " + reply);
    }
}
