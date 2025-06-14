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
    void setEfficiency(double e) {
        energy_efficiency = std::min(e, 100.0);
    }

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

class InsulationUpgrade : public Improvement {
    int insulation_level;
public:
    InsulationUpgrade(int level) : insulation_level(level) {}
    void apply(Building &b) override {
        double boost = std::min(insulation_level * 0.15, 1.0);
        b.setEfficiency(b.getEfficiency() + boost * 25);
        b.setCost(b.getCost() + insulation_level * 400);
    }
    std::string description() const override {
        return "Insulation Upgrade with level " + std::to_string(insulation_level);
    }
};

class WindowReplacement : public Improvement {
    int window_count;
public:
    WindowReplacement(int count) : window_count(count) {}
    void apply(Building &b) override {
        double boost = std::min(window_count * 0.05, 1.0);
        b.setEfficiency(b.getEfficiency() + boost * 10);
        b.setAesthetic(b.getAesthetic() + boost * 5);
        b.setCost(b.getCost() + window_count * 300);
    }
    std::string description() const override {
        return "Window Replacement of " + std::to_string(window_count) + " windows";
    }
};

class GreenRoof : public Improvement {
    double area;
public:
    GreenRoof(double a) : area(a) {}
    void apply(Building &b) override {
        double efficiency_boost = std::min(area * 0.03, 1.0);
        double aesthetic_boost = std::min(area * 0.02, 1.0);
        b.setEfficiency(b.getEfficiency() + efficiency_boost * 15);
        b.setAesthetic(b.getAesthetic() + aesthetic_boost * 10);
        b.setCost(b.getCost() + area * 200);
    }
    std::string description() const override {
        return "Green Roof with area " + std::to_string(area) + " sqm";
    }
};

TEST(BuildingTest, FacadeRenovationOnly) {
    std::vector<Improvement *> plan = { new FacadeRenovation(8) };
    Building b(25.0, 75000, 50, 40, plan);
    b.renovate();
    ASSERT_TRUE(b.getAesthetic() > 40);
    ASSERT_EQ(b.getCost(), 75000 + 8 * 500);
    return true;
}

TEST(BuildingTest, MultipleImprovementsCumulative) {
    std::vector<Improvement *> plan = { new SolarPanels(30), new FacadeRenovation(3), new SolarPanels(20) };
    Building b(35.0, 90000, 55, 65, plan);
    b.renovate();
    ASSERT_TRUE(b.getEfficiency() > 55);
    ASSERT_TRUE(b.getAesthetic() > 65);
    ASSERT_TRUE(b.getCost() > 90000);
    return true;
}

TEST(BuildingTest, NoImprovementAfterDelete) {
    std::vector<Improvement *> plan = { new SolarPanels(40), new FacadeRenovation(5) };
    Building *b = new Building(28.0, 85000, 65, 75, plan);
    delete b;
    ASSERT_TRUE(true);
    return true;
}

TEST(BuildingTest, ZeroImprovementPlan) {
    std::vector<Improvement *> plan;
    Building b(40.0, 120000, 75, 85, plan);
    b.renovate();
    ASSERT_EQ(b.getHeight(), 40.0);
    ASSERT_EQ(b.getCost(), 120000);
    ASSERT_EQ(b.getEfficiency(), 75);
    ASSERT_EQ(b.getAesthetic(), 85);
    return true;
}

TEST(BuildingTest, EfficiencyCap) {
    std::vector<Improvement *> plan = { new SolarPanels(200) };
    Building b(20.0, 50000, 80, 60, plan);
    b.renovate();
    ASSERT_EQ(b.getEfficiency(), 100);
    ASSERT_EQ(b.getCost(), 50000 + 200 * 100);
    return true;
}

TEST(BuildingTest, InsulationUpgradeEffect) {
    std::vector<Improvement *> plan = { new InsulationUpgrade(6) };
    Building b(22.0, 60000, 60, 55, plan);
    b.renovate();
    ASSERT_TRUE(b.getEfficiency() > 60);
    ASSERT_EQ(b.getCost(), 60000 + 6 * 400);
    return true;
}

TEST(BuildingTest, WindowReplacementEffect) {
    std::vector<Improvement *> plan = { new WindowReplacement(10) };
    Building b(30.0, 70000, 70, 50, plan);
    b.renovate();
    ASSERT_TRUE(b.getEfficiency() > 70);
    ASSERT_TRUE(b.getAesthetic() > 50);
    ASSERT_EQ(b.getCost(), 70000 + 10 * 300);
    return true;
}

TEST(BuildingTest, GreenRoofEffect) {
    std::vector<Improvement *> plan = { new GreenRoof(25.0) };
    Building b(18.0, 55000, 65, 55, plan);
    b.renovate();
    ASSERT_TRUE(b.getEfficiency() > 65);
    ASSERT_TRUE(b.getAesthetic() > 55);
    ASSERT_EQ(b.getCost(), 55000 + 25 * 200);
    return true;
}

int main() {
    RUN_TEST(BuildingTest, FacadeRenovationOnly);
    RUN_TEST(BuildingTest, MultipleImprovementsCumulative);
    RUN_TEST(BuildingTest, NoImprovementAfterDelete);
    RUN_TEST(BuildingTest, ZeroImprovementPlan);
    RUN_TEST(BuildingTest, EfficiencyCap);
    RUN_TEST(BuildingTest, InsulationUpgradeEffect);
    RUN_TEST(BuildingTest, WindowReplacementEffect);
    RUN_TEST(BuildingTest, GreenRoofEffect);
    return 0;
}
