/* DO NOT EDIT THIS FILE */
/* This file is autogenerated by the text-database code generator */

#include <stdarg.h>
#include <touchgfx/TextProvider.hpp>
#include <touchgfx/Texts.hpp>
#include <touchgfx/TypedText.hpp>
#include <touchgfx/Unicode.hpp>
#include <touchgfx/hal/HAL.hpp>
#include <touchgfx/lcd/LCD.hpp>
#include <texts/TypedTextDatabase.hpp>

uint16_t touchgfx::Font::getStringWidth(const touchgfx::Unicode::UnicodeChar* text, ...) const
{
    va_list pArg;
    va_start(pArg, text);
    uint16_t width = getStringWidthLTR(TEXT_DIRECTION_LTR, text, pArg);
    va_end(pArg);
    return width;
}

uint16_t touchgfx::Font::getStringWidth(touchgfx::TextDirection textDirection, const touchgfx::Unicode::UnicodeChar* text, ...) const
{
    va_list pArg;
    va_start(pArg, text);
    uint16_t width = getStringWidthLTR(textDirection, text, pArg);
    va_end(pArg);
    return width;
}

touchgfx::Unicode::UnicodeChar touchgfx::TextProvider::getNextLigature(TextDirection direction)
{
    if (fontGsubTable && nextCharacters.peekChar())
    {
        substituteGlyphs();
        if (nextCharacters.peekChar(1) == 0x093F) // Hindi I-matra
        {
            nextCharacters.replaceAt1(nextCharacters.peekChar());
            nextCharacters.replaceAt0(0x093F);
        }
    }
    return getNextChar();
}

void touchgfx::TextProvider::initializeInternal()
{
    fillInputBuffer();
}

void touchgfx::LCD::drawString(touchgfx::Rect widgetArea, const touchgfx::Rect& invalidatedArea, const touchgfx::LCD::StringVisuals& stringVisuals, const touchgfx::Unicode::UnicodeChar* format, ...)
{
    va_list pArg;
    va_start(pArg, format);
    drawStringLTR(widgetArea, invalidatedArea, stringVisuals, format, pArg);
    va_end(pArg);
}

// Default TypedTextDatabase
extern const touchgfx::TypedText::TypedTextData* const typedTextDatabaseArray[];

TEXT_LOCATION_FLASH_PRAGMA
KEEP extern const touchgfx::Unicode::UnicodeChar texts_all_languages[] TEXT_LOCATION_FLASH_ATTRIBUTE = {
    0x43, 0x68, 0x61, 0x6e, 0x6e, 0x65, 0x6c, 0x20, 0x31, 0x20, 0x47, 0x61, 0x69, 0x6e, 0x0, // @0 "Channel 1 Gain"
    0x43, 0x68, 0x61, 0x6e, 0x6e, 0x65, 0x6c, 0x20, 0x32, 0x20, 0x47, 0x61, 0x69, 0x6e, 0x0, // @15 "Channel 2 Gain"
    0x43, 0x68, 0x61, 0x6e, 0x6e, 0x65, 0x6c, 0x20, 0x33, 0x20, 0x47, 0x61, 0x69, 0x6e, 0x0, // @30 "Channel 3 Gain"
    0x43, 0x68, 0x61, 0x6e, 0x6e, 0x65, 0x6c, 0x20, 0x34, 0x20, 0x47, 0x61, 0x69, 0x6e, 0x0, // @45 "Channel 4 Gain"
    0x41, 0x75, 0x64, 0x69, 0x6f, 0x20, 0x45, 0x66, 0x66, 0x65, 0x63, 0x74, 0x73, 0x0, // @60 "Audio Effects"
    0x43, 0x68, 0x61, 0x6e, 0x6e, 0x65, 0x6c, 0x20, 0x31, 0x20, 0x50, 0x61, 0x6e, 0x0, // @74 "Channel 1 Pan"
    0x43, 0x68, 0x61, 0x6e, 0x6e, 0x65, 0x6c, 0x20, 0x32, 0x20, 0x50, 0x61, 0x6e, 0x0, // @88 "Channel 2 Pan"
    0x43, 0x68, 0x61, 0x6e, 0x6e, 0x65, 0x6c, 0x20, 0x33, 0x20, 0x50, 0x61, 0x6e, 0x0, // @102 "Channel 3 Pan"
    0x43, 0x68, 0x61, 0x6e, 0x6e, 0x65, 0x6c, 0x20, 0x34, 0x20, 0x50, 0x61, 0x6e, 0x0, // @116 "Channel 4 Pan"
    0x44, 0x65, 0x6c, 0x61, 0x79, 0x20, 0x4f, 0x6e, 0x2f, 0x4f, 0x66, 0x66, 0x0, // @130 "Delay On/Off"
    0x43, 0x68, 0x61, 0x6e, 0x6e, 0x65, 0x6c, 0x20, 0x50, 0x61, 0x6e, 0x0, // @143 "Channel Pan"
    0x44, 0x65, 0x6c, 0x61, 0x79, 0x20, 0x54, 0x69, 0x6d, 0x65, 0x0, // @155 "Delay Time"
    0x45, 0x71, 0x75, 0x61, 0x6c, 0x69, 0x7a, 0x65, 0x72, 0x0, // @166 "Equalizer"
    0x4f, 0x75, 0x74, 0x20, 0x47, 0x61, 0x69, 0x6e, 0x20, 0x0, // @176 "Out Gain "
    0x46, 0x65, 0x65, 0x64, 0x62, 0x61, 0x63, 0x6b, 0x0, // @186 "Feedback"
    0x50, 0x72, 0x65, 0x20, 0x41, 0x6d, 0x70, 0x0, // @195 "Pre Amp"
    0x57, 0x65, 0x74, 0x6e, 0x65, 0x73, 0x73, 0x0, // @203 "Wetness"
    0x46, 0x61, 0x64, 0x65, 0x72, 0x73, 0x0, // @211 "Faders"
    0x2b, 0x31, 0x32, 0x64, 0x42, 0x0, // @218 "+12dB"
    0x2b, 0x34, 0x30, 0x64, 0x42, 0x0, // @224 "+40dB"
    0x2d, 0x31, 0x32, 0x64, 0x42, 0x0, // @230 "-12dB"
    0x2b, 0x36, 0x64, 0x42, 0x0, // @236 "+6dB"
    0x2d, 0x221e, 0x64, 0x42, 0x0, // @241 "-?dB"
    0x48, 0x69, 0x67, 0x68, 0x0, // @246 "High"
    0x4c, 0x6f, 0x77, 0x0, // @251 "Low"
    0x4d, 0x69, 0x64, 0x0, // @255 "Mid"
    0x4c, 0x0, // @259 "L"
    0x52, 0x0 // @261 "R"
};

TEXT_LOCATION_FLASH_PRAGMA
KEEP extern const uint32_t indicesGb[] TEXT_LOCATION_FLASH_ATTRIBUTE;

// Array holding dynamically installed languages
struct TranslationHeader
{
    uint32_t offset_to_texts;
    uint32_t offset_to_indices;
    uint32_t offset_to_typedtext;
};
static const TranslationHeader* languagesArray[1] = { 0 };

// Compiled and linked in languages
static const uint32_t* const staticLanguageIndices[] = {
    indicesGb
};

touchgfx::LanguageId touchgfx::Texts::currentLanguage = static_cast<touchgfx::LanguageId>(0);
static const touchgfx::Unicode::UnicodeChar* currentLanguagePtr = 0;
static const uint32_t* currentLanguageIndices = 0;

void touchgfx::Texts::setLanguage(touchgfx::LanguageId id)
{
    const touchgfx::TypedText::TypedTextData* currentLanguageTypedText = 0;
    if (id < 1)
    {
        if (languagesArray[id] != 0)
        {
            // Dynamic translation is added
            const TranslationHeader* translation = languagesArray[id];
            currentLanguagePtr = (const touchgfx::Unicode::UnicodeChar*)(((const uint8_t*)translation) + translation->offset_to_texts);
            currentLanguageIndices = (const uint32_t*)(((const uint8_t*)translation) + translation->offset_to_indices);
            currentLanguageTypedText = (const touchgfx::TypedText::TypedTextData*)(((const uint8_t*)translation) + translation->offset_to_typedtext);
        }
        else
        {
            // Compiled and linked in languages
            currentLanguagePtr = texts_all_languages;
            currentLanguageIndices = staticLanguageIndices[id];
            currentLanguageTypedText = typedTextDatabaseArray[id];
        }
    }

    if (currentLanguageTypedText)
    {
        currentLanguage = id;
        touchgfx::TypedText::registerTypedTextDatabase(currentLanguageTypedText,
                                                       TypedTextDatabase::getFonts(), TypedTextDatabase::getInstanceSize());
    }
}

void touchgfx::Texts::setTranslation(touchgfx::LanguageId id, const void* translation)
{
    languagesArray[id] = (const TranslationHeader*)translation;
}

const touchgfx::Unicode::UnicodeChar* touchgfx::Texts::getText(TypedTextId id) const
{
    return &currentLanguagePtr[currentLanguageIndices[id]];
}
