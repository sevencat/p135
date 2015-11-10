#include "stdafx.h"
#include "HttpConnection.h"
#include "status_codes.hpp"
#include <asio/write.hpp>
#include <asio/placeholders.hpp>

namespace King
{
	void http_connection::start()
	{
		socket_.async_read_some(asio::buffer(buffer_),
			boost::bind(&http_connection::handle_read,
			this->shared_from_this(),
			asio::placeholders::error,
			asio::placeholders::bytes_transferred));
	}

	void http_connection::send_response(int status_code, std::string message)
	{
		std::string status_text;
#define HTTP_STATUS_CODE(code, descr, type) \
		case (code): \
			status_text = (descr); \
			break;
		switch (status_code)
		{
			HTTP_STATUS_CODE_MAP(HTTP_STATUS_CODE)
		default:
			status_text = "UNKNOWN";
			break;
		}
#undef HTTP_STATUS_CODE

		std::ostream o(&outgoing_buffer_);
		o << "HTTP/1.1 " << status_code << " " << status_text << "\r\n"
			<< "Content-Length: " << message.length() << "\r\n";
		if (http_should_keep_alive(&parser_) == 1)
		{
			o << "Connection: keep-alive\r\n";
		}
		o << "\r\n";
		o << message;
		asio::async_write(socket_, outgoing_buffer_,
			boost::bind(&http_connection::handle_write, this->shared_from_this(),
			asio::placeholders::error,
			asio::placeholders::bytes_transferred));
	}

	http_connection::~http_connection()
	{
	}

	http_connection::http_connection(asio::io_service& io_service, HttpHandleFunc& handler)
		:handler_(handler)
		, socket_(io_service)
		, parser_()
		, header_state_(HEADER_START)
	{
	}

	void http_connection::handle_write(const asio::error_code& error, size_t bytes_transferred)
	{
		if (error)
		{
			//这里应该close吗？
			//HTTP_SERVER_DEBUG_OUTPUT("Unable to handle request: %s [Errno %d]\n",error.message().c_str(),error.value());
			return;
		}
		//HTTP_SERVER_DEBUG_OUTPUT("Response sent with %lu bytes\n", bytes_transferred);
	}

	int http_connection::on_message_begin(http_parser* parser)
	{
		http_connection * conn = static_cast<http_connection *>(parser->data);
		conn->request_url_.clear();
		conn->headers_.clear();
		return 0;
	}

	int http_connection::on_header_field(http_parser* parser, const char* at, size_t length)
	{
		http_connection * conn = static_cast<http_connection *>(parser->data);
		if (conn->header_state_ == HEADER_START)
		{
			conn->header_field_.append(at, at + length);
		}
		else if (conn->header_state_ == HEADER_VALUE)
		{
			conn->headers_.insert(std::make_pair(conn->header_field_, conn->header_value_));
			conn->header_field_.clear();
			conn->header_value_.clear();
			conn->header_field_.append(at, at + length);
		}
		else if (conn->header_state_ == HEADER_FIELD)
		{
			conn->header_field_.append(at, at + length);
		}
		conn->header_state_ = HEADER_FIELD;
		return 0;
	}

	int http_connection::on_header_value(http_parser* parser, const char* at, size_t length)
	{
		http_connection * conn = static_cast<http_connection *>(parser->data);
		if (conn->header_state_ == HEADER_FIELD)
		{
			conn->header_value_.clear();
			conn->header_value_.append(at, at + length);
		}
		else if (conn->header_state_ == HEADER_VALUE)
		{
			conn->header_value_.append(at, at + length);
		}
		conn->header_state_ = HEADER_VALUE;
		return 0;
	}

	int http_connection::on_headers_complete(http_parser* parser)
	{
		http_connection * conn = static_cast<http_connection *>(parser->data);
		if (conn->header_state_ == HEADER_VALUE)
		{
			conn->headers_.insert(std::make_pair(conn->header_field_, conn->header_value_));
		}
		return 0;
	}

	int http_connection::on_body(http_parser* parser, const char* at, size_t length)
	{
		http_connection * conn = static_cast<http_connection *>(parser->data);
		conn->request_body_.append(at, at + length);
		return 0;
	}

	int http_connection::on_url(http_parser* parser, const char* at, size_t length)
	{
		http_connection * conn = static_cast<http_connection *>(parser->data);
		conn->request_url_.append(at, at + length);
		return 0;
	}

	//这个设计挺不错，delay消息过去
	int http_connection::on_message_complete(http_parser* parser)
	{
		http_connection * conn = static_cast<http_connection *>(parser->data);
		conn->socket_.get_io_service().post(
			boost::bind(&http_connection::process_request, conn->shared_from_this()));
		return 0;
	}

	void http_connection::handle_read(const asio::error_code& e, std::size_t bytes_transferred)
	{
		if (!e && bytes_transferred)
		{
			const char * data = buffer_.data();
			std::size_t nsize = http_parser_execute(&parser_, &settings_, data, bytes_transferred);
			if (nsize != bytes_transferred)
			{
				//HTTP_SERVER_DEBUG_OUTPUT("http parser execute fail %lu/%lu\n", nsize, bytes_transferred);
				socket_.close();
				return;
			}
			start();
		}
		else
		{
			socket_.close();
		}
	}

	void http_connection::process_request()
	{
		(handler_)(this->shared_from_this());
		// Re-initialize parser
		http_parser_init(&parser_, HTTP_REQUEST);
		parser_.data = this;
	}
}
