#include <iostream>
#include <array>
#include <cctype>
#include "alphabet.cpp"
#include "hardware/flash.h"

#define FLASH_TARGET_OFFSET (PICO_FLASH_SIZE_BYTES - FLASH_SECTOR_SIZE)

class Scorecard
{
public:
    Scorecard(std::array<char, 3> initials, int score)
        : initials(initials), score(score) {}

    int calculateWidth()
    {
        int width = 0;
        width += calculateLetterWidth();
        width++;
        width += calculateScoreWidth();
        return width;
    }

    int calculateLetterWidth()
    {
        int width = 0;
        width += Alphabet::letterWidth[initials[0] - 'A'];
        width += Alphabet::letterWidth[initials[1] - 'A'];
        width += Alphabet::letterWidth[initials[2] - 'A'];
        return width + 2;
    }

    int calculateScoreWidth()
    {
        int width = 0;
        int digit1 = score / 10;
        int digit2 = score % 10;
        width += Numbers::numberWidth[digit1];
        width += Numbers::numberWidth[digit2];
        return width + 1;
    }

    std::array<char, 3> getInitials() const
    {
        return initials;
    }

    int getScore() const
    {
        return score;
    }

    void setScore(int newScore)
    {
        score = newScore;
    }

    static void saveToFlash(const std::array<Scorecard, 3> Scorecards)
    { 
        int* p;
        int first_empty_page = -1;
        for (unsigned int page = 0; page < FLASH_SECTOR_SIZE / FLASH_PAGE_SIZE; page++)
        {
            uint32_t addr = XIP_BASE + FLASH_TARGET_OFFSET + (page * FLASH_PAGE_SIZE);
            p = (int *)addr;
            // 0xFFFFFFFF denotes an empty page
            if (*p == -1 && first_empty_page < 0)
            {
                first_empty_page = page;
            }
        }
        uint8_t buf[FLASH_PAGE_SIZE] ={0};
        int i = 0;
        for (const Scorecard& card : Scorecards)
        {
            auto initial = card.getInitials();
            int score = card.getScore();

            buf[i++] = initial[0];
            buf[i++] = initial[1];
            buf[i++] = initial[2];

            memcpy(&buf[i], &score, sizeof(int));
            i += sizeof(int);
        }
        if (first_empty_page < 0)
        {
            uint32_t ints = save_and_disable_interrupts();
            flash_range_erase(FLASH_TARGET_OFFSET, FLASH_SECTOR_SIZE);
            first_empty_page = 0;
            restore_interrupts(ints);
        }
        uint32_t ints = save_and_disable_interrupts();
        flash_range_program(FLASH_TARGET_OFFSET + (first_empty_page * FLASH_PAGE_SIZE), (uint8_t *)buf, FLASH_PAGE_SIZE);
        restore_interrupts(ints);
    }

static std::array<Scorecard, 3> readFromFlash()
{
    std::array<Scorecard, 3> result = {
        Scorecard({'A', 'A', 'A'}, 0),
        Scorecard({'A', 'A', 'A'}, 0),
        Scorecard({'A', 'A', 'A'}, 0)};

    int last_populated_page = -1;

    for (int page = 0; page < FLASH_SECTOR_SIZE / FLASH_PAGE_SIZE; page++)
    {
        uint32_t addr = XIP_BASE + FLASH_TARGET_OFFSET + (page * FLASH_PAGE_SIZE);
        const uint8_t *buf = reinterpret_cast<const uint8_t *>(addr);

        if (*(reinterpret_cast<const uint32_t *>(buf)) != 0xFFFFFFFF)
        {
            last_populated_page = page; 
        }
    }

    if (last_populated_page != -1)
    {
        uint32_t addr = XIP_BASE + FLASH_TARGET_OFFSET + (last_populated_page * FLASH_PAGE_SIZE);
        const uint8_t *buf = reinterpret_cast<const uint8_t *>(addr);
        int i = 0;
        for (int cardIndex = 0; cardIndex < 3; ++cardIndex)
        {
            std::array<char, 3> initial = {
                static_cast<char>(buf[i++]),
                static_cast<char>(buf[i++]),
                static_cast<char>(buf[i++])};
            int score = 0;
            memcpy(&score, &buf[i], sizeof(int));
            i += sizeof(int);
            result[cardIndex] = Scorecard(initial, score);
        }
    }
    return result;
}

    static void insertScore(std::array<Scorecard,3> &savedScores, const Scorecard &newScore){

        if (newScore.getScore() <= savedScores[2].getScore())
            return;
        int pos = 2;
        for (int i = 0; i < 3; ++i)
        {
            if (newScore.getScore() > savedScores[i].getScore())
            {
                pos = i;
                break;
            }
        }

        for (int i = 2; i > pos; --i)
        {
            savedScores[i] = savedScores[i - 1];
        }

        savedScores[pos] = newScore;
    }
    static void ClearFlashScores(){
        std::array<Scorecard, 3> result = {
            Scorecard({'A', 'A', 'A'}, 0),
            Scorecard({'A', 'A', 'A'}, 0),
            Scorecard({'A', 'A', 'A'}, 0)};
        
        saveToFlash(result);
    }

    static int getLowestScore(const std::array<Scorecard, 3> &cards)
    {
        return cards[2].getScore();
    }

private:
    std::array<char, 3> initials;
    int score;
};