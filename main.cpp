////////////////////////////////////////
// { PROGRAM NAME } { VERSION }
// Author:
// License:
// Description:
////////////////////////////////////////

/*
* The comments in this file are here to guide you initially. Note that you shouldn't actually
* write comments that are pointless or obvious in your own code, write useful ones instead!
* See this for more details: https://ce-programming.github.io/toolchain/static/coding-guidelines.html
*
* Have fun!
*/

/* You probably want to keep these headers */
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <tice.h>

/* Here are some standard headers. Take a look at the toolchain for more. */
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <graphx.h>
#include <keypadc.h>
/* Put function prototypes here or in a header (.h) file */

/* Note: uint8_t is an unsigned integer that can range from 0-255. */
/* It performs faster than just an int, so try to use it (or int8_t) when possible */
void printText(const char *text, uint8_t x, uint8_t y);
void gameLoop();
void init();

// The currency system is relative to a constant currency "sGold". It doesn't fluctuate.
// The money is based on 0.001 increments.

class Game { public:
    int64_t integerPrecision = 1000; // One of this value (For example 1000) equals one
    class Currency { public:
        int64_t inflationPerStep; //Subdivided into 0.001% increments, increases when citizens lose trust in currency
        int64_t money = 1000000000; // The money that your government current owns. Uses default Integer Precision
        int64_t totalMoneyPrinted = money; // All of the money that has been printed
        int64_t moneyInCirculation = money; // The money controlled by everyone, people can lose money in the game.
        int64_t moneyValue = 1; // The value of money compared to sGold
        int64_t costForMoneyToPrint; // The cost for money to print in sGold. It decreases when certain techs are researched
    }; Currency Currency;


    // Likely will remain unused
    class BuyableProducts { public:
        const char* label[6] = {"Water", "Food-lowQuality", "Food-medQuality", "Food-highQuality", "Furniture", "Knowledge"};
        int64_t itemCost[6] = {1000, 1000, 10000, 100000, 50000, 0}; // In sGold
    }; BuyableProducts BuyableProducts;

    class City { public:
        bool isOnSea = false;
        uint8_t climateID = 0; // TODO: document

        class Resources { public:
            int foodProduction = 200; // Normal integer precsion

        }; Resources Resources;

        class Budget { public:

            /*
            Sea tile efficiency:
            Import cost - 25% less
            FoodGen - +15% efficiency (Ex. -95% + 15% = -80%
            */

            /*
            Efficiency for climates

            ARCTIC:
            FoodGen/infastructure - -95%
            Everything else: -40%

            SUB-ARCTIC:
            FoodGen/infastructure - -80%
            Everything else: -30%
            */


            /* As a percent of your global budget per year, which is in sGold
            [Food gen budget (dependant on starting location),
            infastructure,
            industry (efficiency increases with population,
            office (efficiency increases with population) effected by tax on wealthy people, also by population),
            businesses (efficiency increases logarithmically with population)
            healthcare (increases pop growth)
            entertainment (increases happiness)
            security (lowers happiness among poor people, increases happiness among middle-class or better)
            */

            // Precision level of 10, note this.
            // In percent form.

            uint64_t totalBudget =    (uint64_t) 200000 * 1000; // Standard precision, sGold
            int foodBudget =          30 * 10;
            int infastructureBudget = 20 * 10;
            int industryBudget =      3 * 10;
            int officeBudget =        2 * 10;
            int businessBudget =      20 * 10;
            int healthcareBudget =    5 * 10;
            int entertainmentBudget = 10 * 10;
            int securityBudget =      10 * 10;

            int FoodEfficiency;
            int infastructureEfficiency;
            int industryEfficiency;
            int officeEfficiency;
            int businessEfficiency;
            int healthcareEfficiency;
            int entertainmentEfficiency;
            int securityEfficiency;
        }; Budget Budget;

        class Taxation { public:
            class WealthTax { public:
                // Corrosponds to Game.City.incomeBrackets.classes
                // TODO: Set to use the integer precision variable.
                int classes[8] = {0, 0, 1000, 1000 * 3, 1000 * 5, 1000 * 4, 1000 * 2};
            }; WealthTax WealthTax;

            // As a percent, default precision
            int businessTax = 6;  // Decay multiplier - 0.9^x
            int industryTax = 5; // Decay multiplier - 0.82^x
            int officeTax   = 4; // Decay multiplier - 0.75^x

        }; Taxation Taxation;
        class IncomeBrackets { public:
            // [Minimum for income bracket, Maximum for income bracket] (Uses sGold)
            // Supposed to use integerPrecision.
            int64_t classes[8][2] = {{0, 1900}, {1900, 30000}, {30000, 90000}, {90000, 145000}, {145000, 300000}, {300000, 900000}, {900000, 3000000}, {3000000, 300000000000}};
        }; IncomeBrackets IncomeBrackets;

        class Population { public:
            int totalPeople = 200;
        }; Population Population;
    }; City City;
};

class Run { public:
    Game Game;
    void simulatePopulation() {
        Game.City.Population.totalPeople += (Game.City.Population.totalPeople - Game.City.Resources.foodProduction) / 5;
    }
    void calculateEfficiencies() {
        // For Food efficiency, refer to the decleration for documentation
        Game.City.Budget.FoodEfficiency = 10 * (Game.City.Budget.foodBudget);
    }
    void calculateServices() {
        Game.City.Resources.foodProduction = Game.City.Budget.FoodEfficiency / 200
    }
    void simulateTurn() {
        calculateEfficiencies();
        simulatePopulation();
    }
};

void renderGUI() {

}

void gameLoop(){

}

int main() {

    boot_Set48MHzMode();

    Game Game;
    gameLoop();

    return 0;
}
