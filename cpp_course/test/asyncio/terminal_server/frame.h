#ifndef CPPCOURSE_TEST_ASYNCIO_TERMINAL_SERVER_FRAME_H
#define CPPCOURSE_TEST_ASYNCIO_TERMINAL_SERVER_FRAME_H
#include <memory>
#include <iostream>
#include <boost/asio.hpp>

//assume format:
// leading 4 byte 0a 0b 0c 0d
// next 2 byte: frame size, which is the size of whole frame.

class Frame{
public:
    //constructor, it is a view
    explicit Frame(boost::asio::streambuf &streambuf);

    //search leading characters, return true if leading is found
    bool trimToLeadingChars();

    //this is the whole size of frame
    //if size cannot be parsed, return max
    size_t parseFrameSize() const;

    //assume data there there
    void print(size_t frameSize, std::ostream &out) const;

    //pStart point to data without leading chars and size
    void writeFrameContent(unsigned char *pStart, size_t dataSize);

private:
    boost::asio::streambuf &streambuf_;

};
#endif //CPPCOURSE_TEST_ASYNCIO_TERMINAL_SERVER_FRAME_H
