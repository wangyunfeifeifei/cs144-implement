#include "stream_reassembler.hh"

// Dummy implementation of a stream reassembler.

// For Lab 1, please replace with a real implementation that passes the
// automated checks run by `make check_lab1`.

// You will need to add private members to the class declaration in `stream_reassembler.hh`

template <typename... Targs>
void DUMMY_CODE(Targs &&.../* unused */) {}

using namespace std;

StreamReassembler::StreamReassembler(const size_t capacity)
    : _buf(deque<char>(capacity, '\0'))
    , _flags(capacity, false)
    , _is_eof(false)
    , _eof_idx(0)
    , _unassembled(0)
    , _output(capacity)
    , _capacity(capacity) {}

//! \details This function accepts a substring (aka a segment) of bytes,
//! possibly out-of-order, from the logical stream, and assembles any newly
//! contiguous substrings and writes them into the output stream in order.
void StreamReassembler::push_substring(const string &data, const size_t index, const bool eof) {
    const size_t _first_unassembled = _output.bytes_written();
    const size_t _start = max(_first_unassembled, index);  // 剪切掉已经被写入_output的部分
    const size_t _end =
        min(index + data.length(), _first_unassembled + _output.remaining_capacity());  // 剪切掉超出容量的尾部

    for (size_t i = _start; i < _end; i++) {
        const size_t idx = i - _first_unassembled;
        if (!_flags[idx]) {  // 没有被部分才写入字节
            _buf[idx] = data[i - index];
            _flags[idx] = true;  // 已经被写入了
            _unassembled++;      // 未装配的字节数加一
        }
    }

    std::string to_write = "";  // 待写入_output的字符
    while (_flags.front()) {    // 直到遇到第一个没有写入的字符停止
        _flags.pop_front();
        to_write += _buf.front();
        _buf.pop_front();

        _flags.push_back(false);
        _buf.push_back('\0');
    }

    _unassembled -= to_write.length();
    _output.write(to_write);

    if (eof) {
        _is_eof = true;
        _eof_idx = index + data.length();  // 达到_eof_idx才算结尾
    }

    if (_is_eof && _eof_idx == _output.bytes_written()) {
        _output.end_input();
    }
}

size_t StreamReassembler::unassembled_bytes() const { return _unassembled; }

bool StreamReassembler::empty() const { return unassembled_bytes() == 0; }
