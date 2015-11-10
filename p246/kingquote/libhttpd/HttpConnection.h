#pragma once
#include <boost/array.hpp>
#include "http_parser.h"

namespace King
{
	class http_connection;
	typedef boost::shared_ptr<http_connection> http_connection_ptr;
	typedef std::function<int(http_connection_ptr)> HttpHandleFunc;
	class http_connection : public boost::enable_shared_from_this<http_connection>
	{
	public:
		typedef boost::shared_ptr<http_connection> pointer;

		static pointer create(asio::io_service& io_service, HttpHandleFunc handler)
		{
			return pointer(new http_connection(io_service, handler));
		}

		asio::ip::tcp::socket& get_socket()
		{
			return socket_;
		}
		void start();
		http_parser_settings settings_;
		/**
		* Send HTTP response.
		*/
		void send_response(int status_code, std::string message);
		inline const std::string & get_request_url() const
		{
			return request_url_;
		}
		typedef std::multimap<std::string, std::string> headers_type;
		const headers_type & get_headers() const
		{
			return headers_;
		}
		int get_request_method() const
		{
			return parser_.method;
		}
		const std::string & get_request_body() const
		{
			return request_body_;
		}
		~http_connection();
	private:
		HttpHandleFunc &handler_;
		http_connection(asio::io_service& io_service,HttpHandleFunc &handler);

		void handle_write(const asio::error_code& error,size_t bytes_transferred);
		asio::ip::tcp::socket socket_;
		/*
		* HTTP stuff
		*/
		http_parser parser_;
		static int on_message_begin(http_parser * parser);
		static int on_header_field(http_parser * parser, const char * at, size_t length);
		static int on_header_value(http_parser * parser, const char * at, size_t length);
		static int on_headers_complete(http_parser * parser);
		std::string request_body_;
		static int on_body(http_parser * parser, const char * at, size_t length);
		/**
		* HTTP parser encountered something that appears to be URL.
		* This callback might be called multiple times, but in our
		* case this gets called just once.
		*/
		static int on_url(http_parser* parser, const char *at, size_t length);
		/**
		* Received complete HTTP request
		*/
		static int on_message_complete(http_parser * parser);
		/**
		* Temporary socket data is stored here.
		*/
		boost::array<char, 8192> buffer_;
		/**
		* Temporary buffer for static responses
		*/
		asio::streambuf outgoing_buffer_;
		/**
		* Received HTTP header.
		*/
		void handle_read(const asio::error_code& e, std::size_t size);
		/**
		* Executes request handler after successful request
		*/
		void process_request();
		/**
		* Full URL of the current request
		*/
		std::string request_url_;
		enum
		{
			HEADER_START,
			HEADER_FIELD,
			HEADER_VALUE
		};
		int header_state_;
		std::string header_field_;
		std::string header_value_;
		headers_type headers_;
	};

}