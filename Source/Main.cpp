// Font serializer by Niall Moody (c) 2010
// Originally posted at http://www.juce.com/forum/topic/typeface-serialisation?page=1
// With modifications/updates (c) 2015 Jonathon Racz

#include "../JuceLibraryCode/JuceHeader.h"

//==============================================================================
int main(int argc, char* argv[])
{
    if (argc != 3)
    {
        std::cout << "Usage: FontSerializer <systemfontname> <outputfilename>\n\n";
        std::cout << "FontSerializer will convert a system-installed font to a portable JUCE-specific format.\n\n";

        return 1;
    }

    // Because we're not using the proper application startup procedure, we need
    // to call this explicitly here to initialise some of the time-related stuff.
    initialiseJuce_GUI();

    // Get file and font name from command line arguments.
    String fontName(argv[1]);
    const File destFile(File::getCurrentWorkingDirectory().getChildFile(argv[2]));

    // Make sure the destination file can be written to.
    OutputStream *destStream = destFile.createOutputStream();
    if (destStream == 0)
    {
        std::cerr << "\nError : Couldn't open " << destFile.getFullPathName() << " for writing." << std::endl;
        return 2;
    }

    // Make sure the font is installed on the current system.
    StringArray fontNames = Font::findAllTypefaceNames();
    if(!fontNames.contains(fontName))
    {
        String error ("\nError: The font " + fontName + " does not exist in the system.\n");
        std::cout << error;
        return 3;
    }

    // Load the font as a system typeface.
    Font font(fontName, 10, 0);
    Typeface::createSystemTypefaceFor(font);

    // Copy the font-properties to a CustomTypeface.
    CustomTypeface customTypeface;
    customTypeface.setCharacteristics(font.getTypefaceName(), font.getAscent(), 
                                      font.isBold(), font.isItalic(), ' ');
    // Here's the important part: copy all glyphs to a new instance of CustomTypeface.
    customTypeface.addGlyphsFromOtherTypeface(*font.getTypeface(), 0, 256);

    // Finally, write the typeface into the destination file.
    customTypeface.writeToStream(*destStream);

    std::cout << "\nWrote font " << fontName << " to file " << destFile.getFullPathName() << " successfully.\n\n";

    delete destStream;

    std::cout << "\n(You might want to use BinaryBuilder to turn this into a C++ file now)\n\n ";

    // This avoids leaking the LookAndFeel instance.
    shutdownJuce_GUI();

    return 0;
}
