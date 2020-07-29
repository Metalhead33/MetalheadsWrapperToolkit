/*
 * File:   StdStream.hpp
 * Author: metalhead33
 *
 * Created on 2017. február 23., 17:46
 */

#ifndef STDSTREAM_HPP
#define STDSTREAM_HPP
#include "../Abstract/Io/FIO.hpp"
#include <cstdio>
#include <string>
class StdStream : public Abstract::FIO {
  private:
    StdStream( const StdStream &cpy ) = delete;      // No copying
    void operator=( const StdStream &cpy ) = delete; // No copying
  public:
    StdStream( );
    StdStream( StdStream &&mov );
    void operator=( StdStream &&mov );
    StdStream( const std::string &newpath, bool ro = false );
    ~StdStream( );
    virtual int64_t read( void *data, int64_t size );
    virtual int64_t seek( int64_t position, SeekPos whence );
    virtual int64_t tell( );
    virtual int64_t size( );
    virtual int64_t write( const void *data, int64_t size );
    virtual char getc( );
    std::string GetPath( );
    bool IsActive( );

    static Abstract::sFIO createReader( const std::string &newpath );
    static Abstract::sFIO createWriter( const std::string &newpath );

    virtual Abstract::byteBuffer loadIntoBuffer( );
    virtual std::string stringize( );

  private:
    typedef std::unique_ptr< FILE, decltype( &fclose ) > HandleType;
    std::string path;
    HandleType chandle;
    bool active;
};
#endif /* STDSTREAM_HPP */
