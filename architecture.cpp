#include <iostream>
#include <vector>
#include <cmath>
#include <string>
#include "test1.h"

class Building;

class Improvement {
public:
    virtual void apply(Building &b) = 0;
    virtual std::string description() const = 0;
    virtual ~Improvement() = default;
};

class Building {
    double height;
    double cost;
    double energy_efficiency;
    double aesthetic_value;
    std::vector<Improvement *> renovation_plan;

public:
    Building(double _height, double _cost, double _efficiency, double _aesthetic, const std::vector<Improvement *> &plan)
        : height(_height), cost(_cost), energy_efficiency(_efficiency), aesthetic_value(_aesthetic), renovation_plan(plan) {}

    double getHeight() const { return height; }
    void setHeight(double h) { height = h; }

    double getCost() const { return cost; }
    void setCost(double c) { cost = c; }

    double getEfficiency() const { return energy_efficiency; }
    void setEfficiency(double e) { energy_efficiency = e; }

    double getAesthetic() const { return aesthetic_value; }
    void setAesthetic(double a) { aesthetic_value = a; }

    void renovate() {
        for (auto *i : renovation_plan) {
            i->apply(*this);
        }
    }

    Building(const Building &) = delete;
    Building &operator=(const Building &) = delete;

    ~Building() {
        for (auto *i : renovation_plan)
            delete i;
    }
};

class SolarPanels : public Improvement {
    int panel_area;
public:
    SolarPanels(int area) : panel_area(area) {}
    void apply(Building &b) override {
        double boost = std::min(panel_area * 0.02, 1.0);
        b.setEfficiency(b.getEfficiency() + boost * 20);
        b.setCost(b.getCost() + panel_area * 100);
    }
    std::string description() const override {
        return "Solar Panels with area " + std::to_string(panel_area) + " sqm";
    }
};

class FacadeRenovation : public Improvement {
    int quality_level;
public:
    FacadeRenovation(int level) : quality_level(level) {}
    void apply(Building &b) override {
        double boost = std::min(quality_level * 0.1, 1.0);
        b.setAesthetic(b.getAesthetic() + boost * 15);
        b.setCost(b.getCost() + quality_level * 500);
    }
    std::string description() const override {
        return "Facade Renovation with quality level " + std::to_string(quality_level);
    }
};

TEST(BuildingTest, Initialization) {
    std::vector<Improvement *> plan = {
        new SolarPanels(50),
        new FacadeRenovation(4)
    };

    Building b(30.0, 100000, 60, 70, plan);

    ASSERT_EQ(b.getHeight(), 30.0);
    ASSERT_EQ(b.getCost(), 100000);
    ASSERT_EQ(b.getEfficiency(), 60);
    ASSERT_EQ(b.getAesthetic(), 70);

    return true;
}

TEST(BuildingTest, RenovationEffect) {
    std::vector<Improvement *> plan = {
        new SolarPanels(50),
        new FacadeRenovation(4)
    };

    Building b(30.0, 100000, 60, 70, plan);
    b.renovate();

    ASSERT_TRUE(b.getEfficiency() > 60);
    ASSERT_TRUE(b.getCost() > 100000);
    ASSERT_TRUE(b.getAesthetic() > 70);

    return true;
}

int main() {
    RUN_TEST(BuildingTest, Initialization);
    RUN_TEST(BuildingTest, RenovationEffect);
    return 0;
}
