#include "Help.hpp"

const char* HelpContents =
//345678901234567890123456789012345678901234567890123456789012345678901234567890
"\n"
"                            Usenet Archive Browser\n"
"                           ========================\n"
"\n"
"                                  Thread view\n"
"                                 -------------\n"
"\n"
"  Each message in archive is displayed in a separate line. An example line\n"
"would look like this:\n"
"\n"
"  g-  42 [Author's name] + Subject of the message          [1995-07-21 14:32]\n"
"  ^^   ^                 ^\n"
"  ||   |                 `-- Indicates folded thread tree.\n"
"  ||   `-------------------- Number of messages in this subtree.\n"
"  |`------------------------ Message flags.\n"
"  `------------------------- Galaxy flags.\n"
"\n"
"  Folded thread trees are indicated with '+' sign. Expanded trees are marked\n"
"with '-'. There is no marker for messages without children.\n"
"\n"
"  The following message flags may be displayed:\n"
"   '-' - indicates no flags set.\n"
"   'r' - this message was already visited (and presumably, read).\n"
"   'R' - this message and all its children were visited.\n"
"\n"
"  In an optional \"archive galaxy\" mode, the following flags may be present:\n"
"   ' ' - indicates no flags set - message is present only on a single group.\n"
"   'x' - this message was posted to more than one newsgroup (crosspost), but\n"
"         it has the same parent message and replies everywhere.\n"
"   'F' - this message has many parents. In most cases it means that the\n"
"         conversation took place in another group and was moved here\n"
"         (followup).\n"
"   '!' - this message has more replies on other groups.\n"
"   '&' - flags 'F' and '!' combined together.\n"
"\n"
"  Note that messages are tracked by their identifiers, so it is perfectly\n"
"normal to enter a previously unvisited newsgroup and have some messages already\n"
"marked as read. This is due to crossposting of the same message on two or more\n"
"groups.\n"
"\n"
"  Currently selected message is indicated by the '->' cursor indicator on the\n"
"left side of the screen. Complementary marker '<' is also displayed on the right\n"
"end. Notice that currently displayed message and message marked by cursor may be\n"
"different.\n"
"\n"
"                                  Keybindings\n"
"                                 -------------\n"
"\n"
"                 q: quit\n"
"    up/down arrows: move cursor\n"
"      page up/down: scroll thread view up/down\n"
"          home/end: move cursor to top/bottom of the screen\n"
"       right arrow: expand thread tree OR move cursor to next message\n"
"        left arrow: collapse thread tree OR move cursor to parent message\n"
"                 p: move cursor to parent message\n"
"                 x: collapse or expand thread tree\n"
"                 e: collapse whole thread and close message view\n"
"                 d: mark message as read and move cursor to next unread one\n"
"             enter: view selected message OR scroll message down one line\n"
"         backspace: scroll message up one line\n"
"             space: view selected message OR scroll message down one screen\n"
"            delete: scroll message up one screen\n"
"                 ,: move cursor to previously viewed message\n"
"                 .: reversal of the above\n"
"                 i: view group information (charter)\n"
"                 c: plot group charts\n"
"                 t: switch between abbreviated and full headers list\n"
"                 r: enable/disable ROT13 encoding\n"
"                 g: go to specified message id\n"
"                 #: go to specified message index\n"
"                 s: search messages for specified text\n"
"                 o: open another archive\n"
"                 w: follow crossposts\n"
"\n"
"                                  Search view\n"
"                                 -------------\n"
"\n"
"  Going to search view will display previous query results. This way you can\n"
"quickly view messages without losing state of search.\n"
"\n"
"                                   Chart view\n"
"                                  ------------\n"
"\n"
"  In the chart view you are presented with a group activity plot. You can see\n"
"how the message numbers changed over the time.\n"
"\n"
"                                     About\n"
"                                    -------\n"
"\n"
"Part of Usenet Archive Toolkit, https://bitbucket.org/wolfpld/usenetarchive\n"
"Copyright 2016 Bartosz Taudul <wolf.pld@gmail.com>\n"
"Licensed under GNU Affero General Public License 3\n"
"\n"
"Usenet Archive Toolkit uses the following libraries: LZ4, xxhash, zstd, LZMA,\n"
"INN, TIN, GMime, terminator, sparsepp, libcurses."
;

const char* SearchHelpContents =
//345678901234567890123456789012345678901234567890123456789012345678901234567890
"\n"
"                            Usenet Archive Browser\n"
"                           ========================\n"
"\n"
"                                  Search view\n"
"                                 -------------\n"
"\n"
"  Enter keywords you wish to search for. Search hints:\n"
"\n"
"  - Quote words to disable fuzzy search.\n"
"  - Prepend word with from: to search for author.\n"
"  - Prepend word with subject: to search in subject.\n"
"  - Prepend word with + to require this word.\n"
"  - Prepend word with - to exclude this word.\n"
"  - Append word with * to search for any word with such beginning.\n"
"\n"
"\n"
"  For example, to require an exact match in a 'from' field, type:\n"
"\n"
"    +from:\"query\"\n"
;

const char* ChartHelpContents =
//345678901234567890123456789012345678901234567890123456789012345678901234567890
"\n"
"                            Usenet Archive Browser\n"
"                           ========================\n"
"\n"
"                                   Chart view\n"
"                                 -------------\n"
"\n"
"  In the chart view you are presented with a group activity plot. You can see\n"
"how the message numbers changed over the time.\n"
"\n"
"  In addition to plotting all messages posted to the newsgroup, you can also\n"
"view result of the search query, which will display trends over time for given\n"
"terms. To do so, press the 's' key and enter what you are searching for. To go\n"
"back to the default (all posts) view, press 'a'. To overlay search results on\n"
"top of all posts press the 'o' key.\n"
"\n"
"  An optional high-resolution mode can be activated by pressing the 'h' key.\n"
"Be aware that it only looks good on a relatively small set of fonts."
"\n"
"  You can view a chart of all messages in the galaxy by pressing the 'g' key.\n"
;
