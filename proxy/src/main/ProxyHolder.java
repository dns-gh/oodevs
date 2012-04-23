package main;

import java.io.IOException;
import java.util.concurrent.locks.Lock;
import java.util.concurrent.locks.ReadWriteLock;
import java.util.concurrent.locks.ReentrantReadWriteLock;

import javax.servlet.ServletConfig;
import javax.servlet.ServletException;
import javax.servlet.ServletRequest;
import javax.servlet.ServletResponse;

import org.eclipse.jetty.servlets.ProxyServlet.Transparent;

public class ProxyHolder {

    private final Transparent proxy_;
    private final String host_;
    private final int port_;
    private final Lock read_;
    private final Lock write_;

    public ProxyHolder(final String prefix, final String host, final int port, final ServletConfig config) throws ServletException {
        proxy_ = new Transparent("/" + prefix, host, port);
        proxy_.init(config);
        host_ = host;
        port_ = port;
        final ReadWriteLock lock = new ReentrantReadWriteLock();
        read_ = lock.readLock();
        write_ = lock.writeLock();
    }

    public String getHost() {
        return host_;
    }

    public int getPort() {
        return port_;
    }

    public void service(final ServletRequest req, final ServletResponse res) throws ServletException, IOException {
        read_.lock();
        try {
            proxy_.service(req, res);
        } finally {
            read_.unlock();
        }
    }

    public void destroy() {
        write_.lock();
        try {
            proxy_.destroy();
        } finally {
            write_.unlock();
        }
    }
}
