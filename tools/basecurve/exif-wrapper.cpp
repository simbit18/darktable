/*
    This file is part of darktable,
                  2014 Edouard Gomez
    Copyright (C) 2025 darktable developers.

    darktable is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    darktable is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with darktable.  If not, see <http://www.gnu.org/licenses/>.
*/

// avoid error reported when including exiv2.hpp on macOS (XCode 15.2)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wshadow"
#include <exiv2/exif.hpp>
#include <exiv2/error.hpp>
#include <exiv2/image.hpp>
#pragma GCC diagnostic pop

#include <cstdio>
#include <cassert>

using namespace std;

extern "C" int
exif_get_ascii_datafield(
  const char* filename,
  const char* key,
  char* buf,
  size_t buflen)
{
  int ret = -1;

  try
  {
    auto image = Exiv2::ImageFactory::open(filename);
    assert(image.get() != 0);
    image->readMetadata();

    Exiv2::ExifData &exifData = image->exifData();

    auto val = exifData[key].getValue();

    if (val->typeId() != Exiv2::asciiString)
    {
      ret = -1;
      goto exit;
    }

    if ((size_t)val->size() >= buflen)
    {
      ret = val->size()+1;
      goto exit;
    }

    snprintf(buf, buflen, "%s", val->toString().c_str());

    ret = 0;
  }
  catch (Exiv2::Error& e)
  {
    ret = -1;
  }

exit:

  return ret;
}

