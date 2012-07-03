package main;

import java.util.Enumeration;

import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletRequestWrapper;

public class ProxyRequest extends HttpServletRequestWrapper {

    final HttpServletRequest root_;

    ProxyRequest(final HttpServletRequest root) {
        super(root);
        root_ = root;
    }

    @Override
    public String getHeader(final String arg0) {
        if (arg0.equalsIgnoreCase("Remote-Address"))
            return root_.getRemoteAddr();
        return root_.getHeader(arg0);
    }

    @Override
    public Enumeration<String> getHeaderNames() {
        return new ProxyEnumeration(root_.getHeaderNames(), "Remote-Address");
    }

    @Override
    public Enumeration<String> getHeaders(final String arg0) {
        final Enumeration<String> base = root_.getHeaders(arg0);
        if (arg0.equalsIgnoreCase("Remote-Address"))
            return new ProxyEnumeration(base, root_.getRemoteAddr());
        return base;
    }

    private class ProxyEnumeration implements Enumeration<String> {
        final Enumeration<String> root_;
        final String bonus_;
        boolean hasBonus_;

        ProxyEnumeration(final Enumeration<String> root, final String bonus) {
            root_ = root;
            bonus_ = bonus;
            hasBonus_ = true;
        }

        @Override
        public boolean hasMoreElements() {
            if (hasBonus_)
                return true;
            return root_.hasMoreElements();
        }

        @Override
        public String nextElement() {
            if (!hasBonus_)
                return root_.nextElement();
            hasBonus_ = false;
            return bonus_;
        }
    };
};
