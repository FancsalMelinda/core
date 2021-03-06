/*
   Copyright (C) Cfengine AS

   This file is part of Cfengine 3 - written and maintained by Cfengine AS.

   This program is free software; you can redistribute it and/or modify it
   under the terms of the GNU General Public License as published by the
   Free Software Foundation; version 3.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA

  To the extent this program is licensed as part of the Enterprise
  versions of Cfengine, the applicable Commerical Open Source License
  (COSL) may apply to this file if you as a licensee so wish it. See
  included file COSL.txt.
*/

#include "cf3.defs.h"
#include "mod_report.h"

static const BodySyntax CF_PRINTFILE_BODY[] =
{
    {"file_to_print", DATA_TYPE_STRING, CF_ABSPATHRANGE, "Path name to the file that is to be sent to standard output"},
    {"number_of_lines", DATA_TYPE_INT, CF_VALRANGE, "Integer maximum number of lines to print from selected file"},
    {NULL, DATA_TYPE_NONE, NULL, NULL}
};

const const BodySyntax CF_REPORT_BODIES[] =
{
    {"friend_pattern", DATA_TYPE_STRING, "", "Regular expression to keep selected hosts from the friends report list"},
    {"intermittency", DATA_TYPE_REAL, "0,1", "Real number threshold [0,1] of intermittency about current peers, report above", "false"},
    {"lastseen", DATA_TYPE_INT, CF_VALRANGE, "Integer time threshold in hours since current peers were last seen, report absence"},
    {"printfile", DATA_TYPE_BODY, CF_PRINTFILE_BODY, "Quote part of a file to standard output"},
    {"report_to_file", DATA_TYPE_STRING, CF_ABSPATHRANGE, "The path and filename to which output should be appended"},
    {"bundle_return_value_index", DATA_TYPE_STRING, CF_IDRANGE, "The promiser is to be interpreted as a literal value that the caller can accept as a result for this bundle, i.e. a return value with array index defined by this attribute."},    
    {"showstate", DATA_TYPE_STRING_LIST, "", "List of services about which status reports should be reported to standard output"},
    {NULL, DATA_TYPE_NONE, NULL}
};

const SubTypeSyntax CF_REPORT_SUBTYPES[] =
{
    /* Body lists belonging to "reports:" type in Agent */

    {"agent", "reports", CF_REPORT_BODIES},
    {NULL, NULL, NULL},
};
