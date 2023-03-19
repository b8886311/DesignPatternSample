#include <iostream>
#include <string>
#include <vector>
#include <memory>
using namespace std;
class IHouseBuilder
{
    public:
        virtual void resetHouse() = 0;
        virtual void buildBedRoom() const = 0;
        virtual void buildLivingRoom() const = 0;
        virtual void buildBathRoom() const = 0;
};
class House
{
    private:
        vector<string> rooms_;
    public:
        void addRoom(string room_type)
        {
            rooms_.emplace_back(room_type);
        }
        vector<string> getRooms()
        {
            return rooms_;
        }
        void printHouseInfo()
        {
            int bedRoom{0}, livingRoom{0}, bathRoom{0};
            for(auto it=rooms_.begin(); it!=rooms_.end(); it++)
            {
                if(*it == "bedRoom") bedRoom++;
                if(*it == "livingRoom") livingRoom++;
                if(*it == "bathRoom") bathRoom++;
            }
            cout << "This house contains\n"
                 << " - " << bedRoom << " bedroom\n"
                 << " - " << livingRoom << " living room\n"
                 << " - " << bathRoom << " bathroom\n";
        }
};
class ApartmentBuilder : public IHouseBuilder
{
    private:
        shared_ptr<House> house_;
    public:
        void startBuildingNewHouse()
        {
            house_ = make_shared<House>();
        }
        void buildBedRoom() const override
        {
            house_->addRoom("bedRoom");
        }
        void buildLivingRoom() const override
        {
            house_->addRoom("livingRoom");
        }
        void buildBathRoom() const override
        {
            house_->addRoom("bathRoom");
        }
        shared_ptr<House> getHouse()
        {
            return move(house_);
        }
        void resetHouse() override
        {
            if(house_ != nullptr)
                house_.reset();
        }
};
class HouseDirector
{
    private:
        shared_ptr<IHouseBuilder> builder_;
    public:
        void setBuilder(shared_ptr<IHouseBuilder> builder)
        {
            builder_ = builder;
        }
        void buildHouseWithBedRoom(int num_bedRoom)
        {
            for(auto i=0; i<num_bedRoom; i++)
                builder_->buildBedRoom();
        }
        void buildHouseWithLivingRoom(int num_livingRoom)
        {
            for(auto i=0; i<num_livingRoom; i++)
                builder_->buildLivingRoom();
        }
        void buildHouseWithBathRoom(int num_BathRoom)
        {
            for(auto i=0; i<num_BathRoom; i++)
                builder_->buildBathRoom();
        }
};
void buildNewHouses(shared_ptr<HouseDirector> director)
{
    auto apartment_builder = make_shared<ApartmentBuilder>();
    {   // User-defined house.
        apartment_builder->startBuildingNewHouse();
        director->setBuilder(apartment_builder);
        director->buildHouseWithBedRoom(2);
        // director->buildHouseWithLivingRoom(0);
        director->buildHouseWithBathRoom(1);
        auto house = apartment_builder->getHouse();
        house->printHouseInfo();
    }
}
int main()
{
    auto director = make_shared<HouseDirector>();
    buildNewHouses(director);
    return 0;
}
