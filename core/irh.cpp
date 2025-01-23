#include <string>
#include <vector>
#include <iostream>

#include <boost/algorithm/string.hpp>
#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/version.hpp>
#include <boost/asio/connect.hpp>
#include <boost/asio/ip/tcp.hpp>

int irhCommand(const std::vector<std::string> &args, const std::vector<std::string> &flags)
{
    if (args.size() < 4)
    {
        return 1;
    }

    std::string request_type_str = args[0];
    std::string url = args[1];
    std::string port = args[2];
    std::string target = args[3];

    boost::to_upper(request_type_str);

    boost::beast::http::verb request_type;

    if (request_type_str == "GET")
    {
        request_type = boost::beast::http::verb::get;
    }
    else if (request_type_str == "POST")
    {
        request_type = boost::beast::http::verb::post;
    }
    else if (request_type_str == "PUT")
    {
        request_type = boost::beast::http::verb::put;
    }
    else if (request_type_str == "DELETE")
    {
        request_type = boost::beast::http::verb::delete_;
    }
    else if (request_type_str == "HEAD")
    {
        request_type = boost::beast::http::verb::head;
    }
    else if (request_type_str == "OPTIONS")
    {
        request_type = boost::beast::http::verb::options;
    }
    else if (request_type_str == "TRACE")
    {
        request_type = boost::beast::http::verb::trace;
    }
    else if (request_type_str == "CONNECT")
    {
        request_type = boost::beast::http::verb::connect;
    }
    else
    {
        return 1;
    }

    try
    {
        boost::asio::io_context ioc;

        boost::asio::ip::tcp::resolver resolver(ioc);
        auto const results = resolver.resolve(url, port);

        boost::beast::tcp_stream stream(ioc);
        stream.connect(results);

        boost::beast::http::request<boost::beast::http::string_body> request{request_type, target, 11}; // HTTP/1.1
        request.set(boost::beast::http::field::host, url);
        request.set(boost::beast::http::field::user_agent, BOOST_BEAST_VERSION_STRING);

        boost::beast::http::write(stream, request);

        boost::beast::flat_buffer buffer;
        boost::beast::http::response<boost::beast::http::dynamic_body> response;
        boost::beast::http::read(stream, buffer, response);

        boost::beast::error_code ec;
        stream.socket().shutdown(boost::asio::ip::tcp::socket::shutdown_both, ec);

        if (ec && ec != boost::beast::errc::not_connected)
        {
            throw boost::beast::system_error{ec};
        }

        std::cout << (response.result_int()) << std::endl;
    }
    catch (std::exception const &e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 1;
}