#include "byte_stream.hh"

// Dummy implementation of a flow-controlled in-memory byte stream.

// For Lab 0, please replace with a real implementation that passes the
// automated checks run by `make check_lab0`.

// You will need to add private members to the class declaration in `byte_stream.hh`

template <typename... Targs>
void DUMMY_CODE(Targs &&... /* unused */) {}

using namespace std;

ByteStream::ByteStream(const size_t capacity)
    : queue(new char[capacity + 1])
    , prev(0)
    , rear(0)
    , _cap(static_cast<int>(capacity))
    , _end(false)
    , sz_read(0)
    , sz_write(0) {}

size_t ByteStream::write(const string &data) {
    size_t len = remaining_capacity() > data.length() ? data.length() : remaining_capacity();
    for (size_t i = 0; i < len; i++) {
        queue[rear] = data[i];
        rear = (rear + 1) % (_cap + 1);
    }
    sz_write += len;
    return len;
}

//! \param[in] len bytes will be copied from the output side of the buffer
string ByteStream::peek_output(const size_t len) const {
    string ret;
    size_t _len = len > buffer_size() ? buffer_size() : len;
    for (size_t i = 0; i < _len; i++) {
        ret += queue[(prev + i) % (_cap + 1)];
    }

    return ret;
}

//! \param[in] len bytes will be removed from the output side of the buffer
void ByteStream::pop_output(const size_t len) {
    size_t _len = len > buffer_size() ? buffer_size() : len;
    for (size_t i = 0; i < _len; i++) {
        prev = (prev + 1) % (_cap + 1);
    }
    sz_read += _len;
}

//! Read (i.e., copy and then pop) the next "len" bytes of the stream
//! \param[in] len bytes will be popped and returned
//! \returns a string
std::string ByteStream::read(const size_t len) {
    string ret = peek_output(len);
    pop_output(len);
    return ret;
}

void ByteStream::end_input() { _end = true; }

bool ByteStream::input_ended() const { return _end; }

size_t ByteStream::buffer_size() const { return (rear - prev + _cap + 1) % (_cap + 1); }

bool ByteStream::buffer_empty() const { return rear == prev; }

bool ByteStream::eof() const { return _end && buffer_empty(); }

size_t ByteStream::bytes_written() const { return sz_write; }

size_t ByteStream::bytes_read() const { return sz_read; }

size_t ByteStream::remaining_capacity() const { return _cap - buffer_size(); }
