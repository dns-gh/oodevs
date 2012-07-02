package main;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.UnsupportedEncodingException;
import java.security.Principal;
import java.util.Collection;
import java.util.Enumeration;
import java.util.Locale;
import java.util.Map;

import javax.servlet.AsyncContext;
import javax.servlet.DispatcherType;
import javax.servlet.RequestDispatcher;
import javax.servlet.ServletContext;
import javax.servlet.ServletException;
import javax.servlet.ServletInputStream;
import javax.servlet.ServletRequest;
import javax.servlet.ServletResponse;
import javax.servlet.http.Cookie;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;
import javax.servlet.http.HttpSession;
import javax.servlet.http.Part;

public class ProxyRequest implements HttpServletRequest {

    final HttpServletRequest root_;

    ProxyRequest(final ServletRequest root) {
        root_ = (HttpServletRequest) root;
    }

    @Override
    public AsyncContext getAsyncContext() {
        return root_.getAsyncContext();
    }

    @Override
    public Object getAttribute(final String arg0) {
        return root_.getAttribute(arg0);
    }

    @Override
    public Enumeration<String> getAttributeNames() {
        return root_.getAttributeNames();
    }

    @Override
    public String getCharacterEncoding() {
        return root_.getCharacterEncoding();
    }

    @Override
    public int getContentLength() {
        return root_.getContentLength();
    }

    @Override
    public String getContentType() {
        return root_.getContentType();
    }

    @Override
    public DispatcherType getDispatcherType() {
        return root_.getDispatcherType();
    }

    @Override
    public ServletInputStream getInputStream() throws IOException {
        return root_.getInputStream();
    }

    @Override
    public String getLocalAddr() {
        return root_.getLocalAddr();
    }

    @Override
    public String getLocalName() {
        return root_.getLocalName();
    }

    @Override
    public int getLocalPort() {
        return root_.getLocalPort();
    }

    @Override
    public Locale getLocale() {
        return root_.getLocale();
    }

    @Override
    public Enumeration<Locale> getLocales() {
        return root_.getLocales();
    }

    @Override
    public String getParameter(final String arg0) {
        return root_.getParameter(arg0);
    }

    @Override
    public Map<String, String[]> getParameterMap() {
        return root_.getParameterMap();
    }

    @Override
    public Enumeration<String> getParameterNames() {
        return root_.getParameterNames();
    }

    @Override
    public String[] getParameterValues(final String arg0) {
        return root_.getParameterValues(arg0);
    }

    @Override
    public String getProtocol() {
        return root_.getProtocol();
    }

    @Override
    public BufferedReader getReader() throws IOException {
        return root_.getReader();
    }

    @SuppressWarnings("deprecation")
    @Override
    public String getRealPath(final String arg0) {
        return root_.getRealPath(arg0);
    }

    @Override
    public String getRemoteAddr() {
        return root_.getRemoteAddr();
    }

    @Override
    public String getRemoteHost() {
        return root_.getRemoteHost();
    }

    @Override
    public int getRemotePort() {
        return root_.getRemotePort();
    }

    @Override
    public RequestDispatcher getRequestDispatcher(final String arg0) {
        return root_.getRequestDispatcher(arg0);
    }

    @Override
    public String getScheme() {
        return root_.getScheme();
    }

    @Override
    public String getServerName() {
        return root_.getServerName();
    }

    @Override
    public int getServerPort() {
        return root_.getServerPort();
    }

    @Override
    public ServletContext getServletContext() {
        return root_.getServletContext();
    }

    @Override
    public boolean isAsyncStarted() {
        return root_.isAsyncStarted();
    }

    @Override
    public boolean isAsyncSupported() {
        return root_.isAsyncSupported();
    }

    @Override
    public boolean isSecure() {
        return root_.isSecure();
    }

    @Override
    public void removeAttribute(final String arg0) {
        root_.removeAttribute(arg0);
    }

    @Override
    public void setAttribute(final String arg0, final Object arg1) {
        root_.setAttribute(arg0, arg1);
    }

    @Override
    public void setCharacterEncoding(final String arg0) throws UnsupportedEncodingException {
        root_.setCharacterEncoding(arg0);
    }

    @Override
    public AsyncContext startAsync() throws IllegalStateException {
        return root_.startAsync();
    }

    @Override
    public AsyncContext startAsync(final ServletRequest arg0, final ServletResponse arg1) throws IllegalStateException {
        return root_.startAsync(arg0, arg1);
    }

    @Override
    public boolean authenticate(final HttpServletResponse arg0) throws IOException, ServletException {
        return root_.authenticate(arg0);
    }

    @Override
    public String getAuthType() {
        return root_.getAuthType();
    }

    @Override
    public String getContextPath() {
        return root_.getContextPath();
    }

    @Override
    public Cookie[] getCookies() {
        return root_.getCookies();
    }

    @Override
    public long getDateHeader(final String arg0) {
        return root_.getDateHeader(arg0);
    }

    @Override
    public String getHeader(final String arg0) {
        if (arg0.equals("Remote-Address"))
            return root_.getRemoteAddr();
        return root_.getHeader(arg0);
    }

    @Override
    public Enumeration<String> getHeaderNames() {
        return new ProxyEnumeration(root_.getHeaderNames(), "Remote-Address");
    }

    @Override
    public Enumeration<String> getHeaders(final String arg0) {
        return new ProxyEnumeration(root_.getHeaders(arg0), root_.getRemoteAddr());
    }

    @Override
    public int getIntHeader(final String arg0) {
        if (arg0.equals("Remote-Address"))
            return -1;
        return root_.getIntHeader(arg0);
    }

    @Override
    public String getMethod() {
        return root_.getMethod();
    }

    @Override
    public Part getPart(final String arg0) throws IOException, ServletException {
        return root_.getPart(arg0);
    }

    @Override
    public Collection<Part> getParts() throws IOException, ServletException {
        return root_.getParts();
    }

    @Override
    public String getPathInfo() {
        return root_.getPathInfo();
    }

    @Override
    public String getPathTranslated() {
        return root_.getPathTranslated();
    }

    @Override
    public String getQueryString() {
        return root_.getQueryString();
    }

    @Override
    public String getRemoteUser() {
        return root_.getRemoteUser();
    }

    @Override
    public String getRequestURI() {
        return root_.getRequestURI();
    }

    @Override
    public StringBuffer getRequestURL() {
        return root_.getRequestURL();
    }

    @Override
    public String getRequestedSessionId() {
        return root_.getRequestedSessionId();
    }

    @Override
    public String getServletPath() {
        return root_.getServletPath();
    }

    @Override
    public HttpSession getSession() {
        return root_.getSession();
    }

    @Override
    public HttpSession getSession(final boolean arg0) {
        return root_.getSession(arg0);
    }

    @Override
    public Principal getUserPrincipal() {
        return root_.getUserPrincipal();
    }

    @Override
    public boolean isRequestedSessionIdFromCookie() {
        return root_.isRequestedSessionIdFromCookie();
    }

    @Override
    public boolean isRequestedSessionIdFromURL() {
        return root_.isRequestedSessionIdFromURL();
    }

    @SuppressWarnings("deprecation")
    @Override
    public boolean isRequestedSessionIdFromUrl() {
        return root_.isRequestedSessionIdFromUrl();
    }

    @Override
    public boolean isRequestedSessionIdValid() {
        return root_.isRequestedSessionIdValid();
    }

    @Override
    public boolean isUserInRole(final String arg0) {
        return root_.isUserInRole(arg0);
    }

    @Override
    public void login(final String arg0, final String arg1) throws ServletException {
        root_.login(arg0, arg1);
    }

    @Override
    public void logout() throws ServletException {
        root_.logout();
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
