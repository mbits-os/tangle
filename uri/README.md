# URI handling library

URI manipulation class, `tangle::uri`.
Contains support for normalization, extracting parts of URIs and parsing and encoding of query strings.

## Properties

The `tangle::uri` contains a couple of properties representing URI components described in [RFC3986](https://tools.ietf.org/html/rfc3986): `scheme`, `authority`, `path`, `query` and `fragment`. Each is implemented as a pair of functions, a getter and a setter. Each setter is named like the component and takes single `tangle::cstring`, each getter is also named like the component and returns a `tangle::cstring`.

## Builders

The `tangle::uri` class contains two helper classes to further parse the URI.

For URLs, the `authority` property can be parsed with a `tangle::uri::auth_builder`. The resulting object allows to read and/or update the parts of authority component: user info, host and port (no work is done to further break the user info):

    auto auth = uri::auth_builder::parse(m_uri.authority());
	...
	m_uri.authority(auth.string());


For any URI supporting the query component, `query` property can be parsed with a `tangle::uri::query_builder`. The `fields` object can be used to add or remove a field or to enumerate which fields there are:

    auto fields = uri::query_builder::parse(m_uri.query());
	...
	m_uri.query(fields.string());

## Utilities

Finaly, the `tangle::uri` class supports a handful of static operations to modify the URIs in one way or another.

The `tangle::uri::make_base` simplifies the task of canonization relative uris. It makes sure the path component of the URI ends with a slash (`/`).

The URI from `make_base` can be used as a second argument in `tangle::uri::canonical`. This function makes a full URI from an identifiers, which might have been in some `href` attribute. As a result, an already absolute URI remains unchanged and a relative onces are calculated in respect to a base URI.

Last one, `tangle::uri::normal`, attempts to get a single representation for an URI out of all the possible representations of this URI. This can be used to help with decision making (whitelisting, blacklisting, etc.)
