# Cookie handling library

HTTP cookies manipulation classes, `tangle::cookie::item` and `tangle::cookie::jar`.
Contains support for parsing, encoding and persistence.

## Client code

For a hypothetical "browsing session", the session should use the `cookie::jar` to keep track of all cookies that have been seen. When a HTTP response would come with `Set-Cookie` header(s), the jar should be updated with `tangle::cookie::jar::from_server()` method,
which will parse the cookie and decide whether or not add the cookie to the jar, based on the criteria from the [RFC2665](https://tools.ietf.org/html/rfc6265).

When sending back the request, the session should consult the `cookie::jar`, which cookies to send back. For `http://example.com/path` URLs, the `tangle::cookie::jar::str()` should be called with

    cookies = m_jar.str({"example.com", "/path"}, match::http);

and for the `https://example.com/path` URLs, the `tangle::cookie::jar::str()` should be called with

    cookies = m_jar.str({"example.com", "/path"}, match::http | match::secure);

If the code using this library allows for a non-network cookie manipulation, it should use a direct calls to `tangle::cookie::jar::add()` when adding a cookie created off-line, with `for_http` parameter set to false and it should never add `tangle::cookie::match::http`, when creating cookie list to read by an off-line, external code.

### Persistence

For persistent cookies the session should have a policy of pruning and storing the file, either periodically or after each request/response. The `tangle::cookie::jar::prune()` will remove the already-expired cookies, so other operations, like `jar::str()`, `jar::add()` and `jar::store()` should be faster. The persistent cookies should generally live longer, than browsing session, so the `tangle::cookie::jar::store()` (or `tangle::cookie::jar::store_raw()`) should be called from time to time. this, in turn, requires that at the beginning of the session either `tangle::cookie::jar::load()` or `tangle::cookie::jar::load_raw()` be called to retrieve persistent cookies from the storage.

## Server

While there is no server version of `cookie::jar`, the `cookie::item` has tools for server-side code as well.

The server wanting to send a cookie to the client can populate an `cookie::item` with the data and call `tangle::cookie::item::server_string()` to create a `Set-Cookie` header. On getting a request with a `Cookie` header, it may want to parse it with `tangle::cookie::from_client()` to get the list of cookies sent by the client.
