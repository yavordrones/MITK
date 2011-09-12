//
// ZipArchive.cpp
//
// $Id$
//
// Library: Zip
// Package: Zip
// Module:  ZipArchive
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


#include "Poco/Zip/ZipArchive.h"
#include "Poco/Zip/SkipCallback.h"
#include "Poco/Exception.h"
#include <cstring>


namespace Poco {
namespace Zip {


ZipArchive::ZipArchive(std::istream& in):
	_entries(),
	_infos(),
	_disks()
{
	poco_assert_dbg (in);
	SkipCallback skip;
	parse(in, skip);
}


ZipArchive::ZipArchive(const FileHeaders& entries, const FileInfos& infos, const DirectoryInfos& dirs):
	_entries(entries),
	_infos(infos),
	_disks(dirs)
{
}


ZipArchive::ZipArchive(std::istream& in, ParseCallback& pc):
	_entries(),
	_infos(),
	_disks()
{
	poco_assert_dbg (in);
	parse(in, pc);
}


ZipArchive::~ZipArchive()
{
}


void ZipArchive::parse(std::istream& in, ParseCallback& pc)
{
	// read 4 bytes
	while (in.good() && !in.eof())
	{
		char header[ZipCommon::HEADER_SIZE]={'\x00', '\x00', '\x00', '\x00'};
		in.read(header, ZipCommon::HEADER_SIZE);
		if (in.eof())
			return;
		if (std::memcmp(header, ZipLocalFileHeader::HEADER, ZipCommon::HEADER_SIZE) == 0)
		{
			ZipLocalFileHeader entry(in, true, pc);
			poco_assert (_entries.insert(std::make_pair(entry.getFileName(), entry)).second);
		}
		else if (std::memcmp(header, ZipFileInfo::HEADER, ZipCommon::HEADER_SIZE) == 0)
		{
			ZipFileInfo info(in, true);
			FileHeaders::iterator it = _entries.find(info.getFileName());
			if (it != _entries.end())
			{
				it->second.setStartPos(info.getRelativeOffsetOfLocalHeader());
			}
			poco_assert (_infos.insert(std::make_pair(info.getFileName(), info)).second);
		}
		else if (std::memcmp(header, ZipArchiveInfo::HEADER, ZipCommon::HEADER_SIZE) == 0)
		{
			ZipArchiveInfo nfo(in, true);
			poco_assert (_disks.insert(std::make_pair(nfo.getDiskNumber(), nfo)).second);
		}
		else
		{
			if (_disks.empty())
				throw Poco::IllegalStateException("Illegal header in zip file");
			else
				throw Poco::IllegalStateException("Garbage after directory header");
		}
	}
}


} } // namespace Poco::Zip
