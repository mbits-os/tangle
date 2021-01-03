# URI handling library

URI manipulation class, `tangle::uri`.
Contains support for normalization, extracting parts of URIs and parsing and encoding of query strings.

## Properties

The `tangle::uri` contains a couple of properties representing URI components described in [RFC3986](https://tools.ietf.org/html/rfc3986): `scheme`, `authority`, `path`, `query` and `fragment`. Each is implemented as a pair of functions, a getter and a setter. Each setter is named like the component and takes single `std::string_view`, each getter is also named like the component and returns a `std::string_view`.

## Builders

The `tangle::uri` class contains two helper classes to further parse the URI.

For URLs, the `authority` property can be parsed with either a `tangle::uri::auth_parts` or more directly with `uri::parsed_authority`. The resulting object allows to read and/or update the parts of authority component: user name, password, host and port:

    auto auth = uri::auth_parts::parse(m_uri.authority());
    ...
    m_uri.authority(auth.string()); // will remove password, if any
    m_uri.authority(auth.string(uri::with_pass)); // will preserve the password

    //////////////////////////////

    auto auth = m_uri.parsed_authority();
    ...
    m_uri.authority(auth.string()); // will remove password, if any
    m_uri.authority(auth.string(uri::with_pass)); // will preserve the password


For any URI supporting the query component, `query` property can be parsed with either a `tangle::uri::params` or more directly with `uri::parsed_query`. The `fields` object can be used to add or remove a field or to enumerate which fields there are:

    auto fields = uri::params::parse(m_uri.query());
    ...
    m_uri.query(fields.string());

    //////////////////////////////

    auto fields = m_uri.parsed_query();
    ...
    m_uri.query(fields.string());

To allow for manipulation and update, both `tangle::uri::auth_parts` and `tangle::uri::params` use `std::string`s for their members.

## Utilities

Finaly, the `tangle::uri` class supports a handful of static operations to modify the URIs in one way or another.

The `tangle::uri::make_base` simplifies the task of canonization relative uris. It makes sure the path component of the URI ends with a slash (`/`).

The URI from `make_base` can be used as a second argument in `tangle::uri::canonical`. This function makes a full URI from an identifiers, which might have been in some `href` attribute. As a result, an already absolute URI remains unchanged and a relative onces are calculated in respect to a base URI.

Last one, `tangle::uri::normal`, attempts to get a single representation for an URI out of all the possible representations of this URI. This can be used to help with decision making (positive filtering, negative filtering, etc.)

Both `tangle::uri::canonical` and `tangle::uri::normal` use `tangle::uri::auth_parts` internally, so both of them are able to take a `tangle::uri::auth_flag` to decide, what to do with the password in the URI.