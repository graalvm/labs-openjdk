#
# ----------------------------------------------------------------------------------------------------
#
# Copyright (c) 2025, Oracle and/or its affiliates. All rights reserved.
# DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS FILE HEADER.
#
# This code is free software; you can redistribute it and/or modify it
# under the terms of the GNU General Public License version 2 only, as
# published by the Free Software Foundation.  Oracle designates this
# particular file as subject to the "Classpath" exception as provided
# by Oracle in the LICENSE file that accompanied this code.
#
# This code is distributed in the hope that it will be useful, but WITHOUT
# ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
# FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
# version 2 for more details (a copy is included in the LICENSE file that
# accompanied this code).
#
# You should have received a copy of the GNU General Public License version
# 2 along with this work; if not, write to the Free Software Foundation,
# Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA.
#
# Please contact Oracle, 500 Oracle Parkway, Redwood Shores, CA 94065 USA
# or visit www.oracle.com if you need additional information or have any
# questions.
#
# ----------------------------------------------------------------------------------------------------

"""
Updates the values for the `version` field of the labsjdk-ce-latest and labsjdk-ee-latest
objects in the common.json file of the Graal repos.
"""

import argparse
import json
import difflib
from pathlib import Path

if __name__ == "__main__":
    parser = argparse.ArgumentParser(description="Updates jdks.labsjdk-ce-latest.version and jdks.labsjdk-ee-latest.version values in common.json.")
    parser.add_argument("labsjdk_versions", action="store", help="URL or path to file from which labsjdk versions will be read. " \
                        "The content must be a JSON object with ce and ee fields that specify the new values.")
    parser.add_argument("common.json", action="store", nargs="+", help="path to a common.json file to process")

    args = parser.parse_args()

    if args.labsjdk_versions.startswith("http://") or args.labsjdk_versions.startswith("https://"):
        from urllib import request
        url = args.labsjdk_versions
        with request.urlopen(url, timeout=10) as response:
            if response.status != 200:
                raise SystemExit(f"Received non-200 status code {response.status} reading {url}")
            content_bytes = response.read()
            encoding = response.info().get_content_charset() or 'utf-8'
            new_versions = content_bytes.decode(encoding)
    else:
        new_versions = Path(args.labsjdk_versions).read_text()

    try:
        new_versions = json.loads(new_versions)
    except json.decoder.JSONDecodeError as e:
        size = len(new_versions)
        if size > 1024:
            new_versions = new_versions[0:1024] + "... (truncated)"
        raise SystemExit(f"Error decoding content of size {size} from {args.labsjdk_versions} as JSON: {e}\nContent:\n{new_versions}")

    for common_json_path in getattr(args, "common.json"):
        common_json_path = Path(common_json_path)
        common_json_text = common_json_path.read_text()
        common_json = json.loads(common_json_text)
        ce_version = common_json["jdks"]["labsjdk-ce-latest"]["version"]
        ee_version = common_json["jdks"]["labsjdk-ee-latest"]["version"]

        try:
            new_common_json_text = common_json_text\
                .replace(ce_version, new_versions["ce"])\
                .replace(ee_version, new_versions["ee"])
        except KeyError as e:
            raise SystemExit(f"Error extracting versions from JSON in {args.labsjdk_versions}: Missing value for {e}\nJSON:\n{json.dumps(new_versions, indent=2)}")

        if new_common_json_text == common_json_text:
            print(f"No change to {common_json_path}")
        else:
            patch = "\n".join((line for line in difflib.unified_diff(
                common_json_text.split("\n"),
                new_common_json_text.split("\n"),
                fromfile=f"a/{common_json_path.name}",
                tofile=f"b/{common_json_path.name}",
                lineterm=""
            )))
            print(f"Updated {common_json_path} with this patch:\n{patch}")
            common_json_path.write_text(new_common_json_text)
