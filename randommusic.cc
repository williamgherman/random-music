/*
 * randommusic
 * William Gherman
 * 2020-02-29
 *
 * Produces PDF score via lilypond of pseudo-random 17-bar musical material for
 * flute and celesta. Requires lilypond
 *
 */

#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <cstdlib>
#include <ctime>

std::string getnotes(const std::vector<std::string> &octaves)
{
    int duration = 0;
    std::string curr_rhythm = "4";
    std::string result = "";
    std::string curr_dynamic, temp_dynamic;
    std::string note;
    const std::vector<std::string> notes =
        {"c", "cis", "d", "es", "e", "f",
         "fis", "g", "aes", "a", "bes", "b", "r", "r"};
    const std::vector<std::string> rhythms = {"2", "4", "8", "16"};
    const std::vector<std::string> dynamics = {"\\pp", "\\p", "\\mp", "\\mf",
                                         "\\f", "\\ff"};

    for (int i = 0; i < 256; i++) {
        if (i == 0)
            note = notes[std::rand()%(notes.size()-2)];
        else
            note = notes[std::rand()%notes.size()];
        if (note != "r")
            note += octaves[std::rand()%octaves.size()];
        if (i == 0 || std::rand() % 3 == 0) {
            curr_rhythm = rhythms[std::rand()%rhythms.size()];
            note += curr_rhythm;
        }
        if (i != 0 && note != "r" && std::rand()%10 == 0) {
            while (temp_dynamic == curr_dynamic)
                temp_dynamic = dynamics[std::rand()%dynamics.size()];
            curr_dynamic = temp_dynamic;
            note += curr_dynamic;
        } else if (i == 0) {
            curr_dynamic = temp_dynamic = dynamics[std::rand()%dynamics.size()];
            note += curr_dynamic;
        }
        result += note;
        result += " ";
        duration += 16 / std::stoi(curr_rhythm, nullptr);
        if (duration >= 255) {
            while (duration < 272) {
                if (duration == 256) {
                    result += "r1 ";
                    duration += 16;
                }
                else if ((272 - duration) % 8 == 0) {
                    result += "r2 ";
                    duration += 8;
                } else if ((272 - duration) % 4 == 0) {
                    result += "r4 ";
                    duration += 4;
                } else if ((272 - duration) % 2 == 0) {
                    result += "r8 ";
                    duration += 2;
                } else {
                    result += "r16 ";
                    duration += 1;
                }
            }
            break;
        }
    }
    return result += "\\bar \"|.\"";
}

int main()
{
    std::srand(std::time(0));
    std::ofstream out("music.ly");
    if (!out) {
        std::cerr << "Could not write to music.ly" << std::endl;
        return 1;
    }
    std::vector<std::string> fl_octaves = {"'", "''", "'''"},
                             upper_octaves = {"'", "''"},
                             lower_octaves = {""};


    out << "\\version \"2.18.2\"\n"
           "\\header {\ntitle = \"Techniques Worksheet 9\"\n"
           "composer = \"William Gherman\"\n}\n"
           "global= {\n\\numericTimeSignature\n\\time 4/4\n\\key c \\major\n}\n"

           "flute = \\new Voice \\with {\n"
           "  \\remove \"Note_heads_engraver\"\n"
           "  \\consists \"Completion_heads_engraver\"\n"
           "  \\remove \"Rest_engraver\"\n"
           "  \\consists \"Completion_rest_engraver\"\n"
           "}\n"
           "\\absolute {\n"
           "  \\set staff.instrumentName = #\"Flute \"\n\n"
        << getnotes(fl_octaves)
        << "\n}\n"
           "upper = \\new Voice \\with {\n"
           "  \\remove \"Note_heads_engraver\"\n"
           "  \\consists \"Completion_heads_engraver\"\n"
           "  \\remove \"Rest_engraver\"\n"
           "  \\consists \"Completion_rest_engraver\"\n"
           "}\n"
           "\\absolute {\n\n"
        << getnotes(upper_octaves)
        << "\n}\n"
           "lower = \\new Voice \\with {\n"
           "  \\remove \"Note_heads_engraver\"\n"
           "  \\consists \"Completion_heads_engraver\"\n"
           "  \\remove \"Rest_engraver\"\n"
           "  \\consists \"Completion_rest_engraver\"\n"
           "}\n"
           "\\absolute {\n"
           "\\clef bass\n\n"
        << getnotes(lower_octaves)
        << "\n}\n"
           "\\score {\n"
           "<<\n"
           "  \\new Staff \\with {\n"
           "    instrumentName = #\"Flute \"\n"
           "  }\n"
           "  {\\global \\flute}\n"
           "  \\new PianoStaff \\with {\n"
           "    instrumentName = #\"Celesta \"\n"
           "  } <<\n"
           "  \\new Staff = \"upper\" {\\global \\upper}\n"
           "  \\new Staff = \"lower\" {\\global \\lower} >>\n"
           ">>\n"
           "\\layout { }\n"
           "}\n"
        << std::endl;
    std::system("lilypond -fpdf music.ly");
    return 0;
}
