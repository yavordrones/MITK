//
// ZipFileInfo.cpp
//
// $Id$
//
// Library: Zip
// Package: Zip
// Module:  ZipFileInfo
//
// Copyright (c) 2007, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// Permission is hereby granted, free of charge, to any person or organization
// obtaining a copy of the software and accompanying documentation covered by
// this license (the "Software") to use, reproduce, display, distribute,
// execute, and transmit the Software, and to prepare derivative works of the
// Software, and to permit third-parties to whom the Software is furnished to
// do so, all subject to the following:
// 
// The copyright notices in the Software and this entire statement, including
// the above license grant, this restriction and the following disclaimer,
// must be included in all copies of the Software, in whole or in part, and
// all derivative works of the Software, unless such copies or derivative
// works are solely in the form of machine-executable object code generated by
// a source language processor.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT
// SHALL THE COPYRIGHT HOLDERS OR ANYONE DISTRIBUTING THE SOFTWARE BE LIABLE
// FOR ANY DAMAGES OR OTHER LIABILITY, WHETHER IN CONTRACT, TORT OR OTHERWISE,
// ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
// DEALINGS IN THE SOFTWARE.
//


#include "Poco/Zip/ZipFileInfo.h"
#include "Poco/Zip/ZipLocalFileHeader.h"
#include "Poco/Buffer.h"
#include <istream>
#include <cstring>


namespace Poco {
namespace Zip {


const char ZipFileInfo::HEADER[ZipCommon::HEADER_SIZE] = {'\x50', '\x4b', '\x01', '\x02'};


ZipFileInfo::ZipFileInfo(const ZipLocalFileHeader& header):
	_rawInfo(),
	_crc32(0),
	_compressedSize(0),
	_uncompressedSize(0),
	_fileName(),
	_lastModifiedAt(),
	_extraField()
{
	std::memset(_rawInfo, 0, FULLHEADER_SIZE);
	std::memcpy(_rawInfo, HEADER, ZipCommon::HEADER_SIZE);
	setCompressedSize(header.getCompressedSize());
	setUncompressedSize(header.getUncompressedSize());
	setCRC(header.getCRC());
	setCompressionMethod(header.getCompressionMethod());
	setCompressionLevel(header.getCompressionLevel());
	setRequiredVersion(header.getMajorVersionNumber(), header.getMinorVersionNumber());
	setHostSystem(header.getHostSystem());
	setLastModifiedAt(header.lastModifiedAt());
	setEncryption(false);
	setFileName(header.getFileName());

	if (getHostSystem() == ZipCommon::HS_UNIX)
		setUnixAttributes();
}


ZipFileInfo::ZipFileInfo(std::istream& in, bool assumeHeaderRead):
	_rawInfo(),
	_crc32(0),
	_compressedSize(0),
	_uncompressedSize(0),
	_fileName(),
	_lastModifiedAt(),
	_extraField()
{
	// sanity check
	poco_assert_dbg (RELATIVEOFFSETLOCALHEADER_POS + RELATIVEOFFSETLOCALHEADER_SIZE == FULLHEADER_SIZE);
	parse(in, assumeHeaderRead);
}


ZipFileInfo::~ZipFileInfo()
{
}


void ZipFileInfo::parse(std::istream& inp, bool assumeHeaderRead)
{
	if (!assumeHeaderRead)
	{
		inp.read(_rawInfo, ZipCommon::HEADER_SIZE);
	}
	else
	{
		std::memcpy(_rawInfo, HEADER, ZipCommon::HEADER_SIZE);
	}
	poco_assert (std::memcmp(_rawInfo, HEADER, ZipCommon::HEADER_SIZE) == 0);
	// read the rest of the header
	inp.read(_rawInfo + ZipCommon::HEADER_SIZE, FULLHEADER_SIZE - ZipCommon::HEADER_SIZE);
	_crc32 = getCRCFromHeader();
	_compressedSize = getCompressedSizeFromHeader();
	_uncompressedSize = getUncompressedSizeFromHeader();
	parseDateTime();
	Poco::UInt16 len = getFileNameLength();
	Poco::Buffer<char> buf(len);
	inp.read(buf.begin(), len);
	_fileName = std::string(buf.begin(), len);
	if (hasExtraField())
	{
		len = getExtraFieldLength();
		Poco::Buffer<char> xtra(len);
		inp.read(xtra.begin(), len);
		_extraField = std::string(xtra.begin(), len);
	}
	len = getFileCommentLength();
	if (len > 0)
	{
		Poco::Buffer<char> buf2(len);
		inp.read(buf2.begin(), len);
		_fileComment = std::string(buf2.begin(), len);
	}
}


std::string ZipFileInfo::createHeader() const
{
	std::string result(_rawInfo, FULLHEADER_SIZE);
	result.append(_fileName);
	result.append(_extraField);
	result.append(_fileComment);
	return result;
}


void ZipFileInfo::setUnixAttributes()
{
	bool isDir = isDirectory();
	int mode;
	if (isDir)
		mode = DEFAULT_UNIX_DIR_MODE;
	else
		mode = DEFAULT_UNIX_FILE_MODE;
	Poco::UInt32 attrs = (mode << 16) | (isDir ? 0x10 : 0);
	setExternalFileAttributes(attrs);
}


} } // namespace Poco::Zip
