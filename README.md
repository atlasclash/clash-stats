**Clash-Stats** is a simple command-line utility for Mac OS and Windows that aggregates and reports Clan War statistics from the Supercell game, Clash of Clans.

# About

Clash-Stats is a command-line tool that reads war data from a comma-separated-value (CSV) file.  Once read, the tool can perform a variety of simple reporting tasks - final score, stars earned by players, and who hit above (snipe) or below (nuke) their townhall level.

Clash-Stats records a player history at each townhall level.  For example, attacks at TH9 are collected and separate from TH10.  Defensive statistics are also reported at each townhall level.

The war results can be written to a SQLite3 database for aggregate reports.

# Supported Platforms

Clash-Stats builds and has been tested on Windows 7 and Mac OS X (10.10+).

# Development Requirements

Building Clash-Stats is pretty easy.  The code is written in portable C++ using
Boost and the C++ STL.  There are project files for Visual Studio 2015 and Xcode 7.2.

1. Clone this repository.
2. Grab [Boost](http://www.boost.org/) binaries or source code for your platform.
   - Mac: 
       - Install [Brew](https://brew.sh/).
       - Open the terminal and type `brew install boost`
       - Optionally, adjust the Xcode project's search paths
3. Build!

# War Input Format

Clash-Stats parses a simple CSV input file.  The file parser is created via a factory method so it is relatively easy to extend and modify the file format.  Currently, the format is composed of three main "sections" of data: header, participants, and attack log.  These are described in detail below.

Examples of the file format are also included in the repository under "wardata".

The maximum width of the file is 7 columns (G).

## Header Block

This section defines the participants in the war, size of the war party and date of the encounter.  Fields not specified can be left blank or ignored.

B1: version number of the file. 

A2: Opponent Clan Name
B2: Opponent Clan Tag
D2: Your Clan Name
E2: Your Clan Tag
G2: User Meta Data (used for search/tagging)

A3: War size

A4: Date of war (YYYY/MM/DD format)

## Participants

The participants section provides the player names, player tags, and townhall levels of all the participants in the war.  An optional flag can be placed in column E to denote a "max" base.

The tool expects a number of rows equal to the war size provided in the header block in cell A3.

A5: War Map Rank
B5: Player Name (home team)
C5: Player Townhall Level
D5: Player Tag
E5: "Max" Flag (1 = Max, empty is okay)
F5: Opponent Townhall Level

## Attack Log

The attack log section lists all of the attacks from the War Log tab in chronological order.  The format is as follows:

Ax: Attack Number
Bx: Home Team Stars
Cx: Home Team Destruction Percentage
Dx: Home Team Player Rank
Ex: Opponent Team Player Rank
Fx: Opponent Team Stars
Gx: Opponent Team Destruction Percentage

The important thing to note is that not all of the cells in each row will have values.  If the attack was initiated by your team, A, B, C, D, and E will have values.  If the attack was initiated by your opponent, A, D, E, F, and G will have values.

# Sample Output

Some of the statistics that Clash-Stats can report are:

* Total Wars
* Total Stars
* Average Stars
* Average Destruction
* 3-Stars
* Peer Attacks/Defends (e.g. TH9 vs TH9)
* Nuke Attacks/Defends (e.g. TH9 vs TH7)
* Snipe Attacks/Defends (e.g. TH9 vs TH10)

