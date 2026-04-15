#include <iostream>
#include <fstream>

enum MarketPlaceType { grocery, homeGoods, stuff };
enum BusinessType  { primary, auxiliary, franchise };


struct TypesInfo {
    MarketPlaceType marketPlaceType;
    BusinessType businessType;
};
struct MarketPlace {
    std::string name;
    int countPVZ;
    std::string city;
    double price;
    TypesInfo typesInfo;


    void updateMarketPlace(int newCountPVZ, double newPrice, MarketPlaceType newType, BusinessType newBusiness) {
        this->countPVZ = newCountPVZ;
        this->price = newPrice;
        this->typesInfo.marketPlaceType = newType;
        this->typesInfo.businessType = newBusiness;
    }

    void print() {
        std::cout << this->name << " " << this->countPVZ << " " << this->city << " " << this->price << " " << this->typesInfo.marketPlaceType << " " << this->typesInfo.businessType << std::endl;
    };
};

struct MarketArray {
    MarketPlace* data = nullptr;
    int length = 0;


    void add(MarketPlace newItem) {
        MarketPlace* newData = new MarketPlace[length + 1];

        for (int i = 0; i < length; i++) {
            newData[i] = data[i];
        }

        newData[length] = newItem;

        delete[] data;

        data = newData;
        length++;
    }
    void copy(MarketArray& arr2) {
        if (arr2.data != nullptr) {
            delete[] arr2.data;
        }
        arr2.data = new MarketPlace[length];
        arr2.length = length;
        for (int i = 0; i < length; i++) {
            arr2.data[i] = data[i];
        }

    }
    void copyByMarketPlaceTypeTo(MarketArray& arr2, MarketPlaceType type) {
        int count = 0;
        for (int i = 0; i < length; i++) {
            if (data[i].typesInfo.marketPlaceType == type) {
                count++;
            }
        }

        if (arr2.data != nullptr) {
            delete[] arr2.data;
        }

        if (count == 0) {
            return;
        }

        arr2.data = new MarketPlace[count];
        arr2.length = 0;

        for (int i = 0; i < length; i++) {
            if (data[i].typesInfo.marketPlaceType == type) {
                arr2.data[arr2.length] = data[i];
                arr2.length++;
            }
        }
    }
    void sortByCountPVZ() {
        for (int i = 0; i < length - 1; i++) {
            for (int j = 0; j < length - i - 1; j++) {
                if (data[j].countPVZ < data[j + 1].countPVZ) {
                    MarketPlace temp = data[j];
                    data[j] = data[j + 1];
                    data[j + 1] = temp;
                }
            }
        }
    }
    void printTop3() {
        MarketArray temp;
        this->copy(temp);
        temp.sortByCountPVZ();
        for (size_t i = 0; i < 3; i++)
        {
            temp.data[i].print();
        }
    }

    void copyNonKazanTo(MarketArray& arr2) {
        int count = 0;
        for (int i = 0; i < length; i++) {
            if (data[i].city != "Казань") {
                count++;
            }
        }

        if (arr2.data != nullptr) {
            delete[] arr2.data;
        }

        if (count == 0) {
            return;
        }

        arr2.data = new MarketPlace[count];
        arr2.length = 0;

        for (int i = 0; i < length; i++) {
            if (data[i].city != "Казань") {
                arr2.data[arr2.length] = data[i];
                arr2.length++;
            }
        }
    }
    void printNonKazan() {
        MarketArray filtered;
        copyNonKazanTo(filtered);

        filtered.print();
    }





    void print() {
        for (size_t i = 0; i < length; i++)
        {
            data[i].print();
        }
    }
    void print(std::string name) {
        for (size_t i = 0; i < length; i++)
        {
            if (name == data[i].name) {
                data[i].print();
            }
        }
    }


    void saveToText()  {
        std::ofstream fout("marketplaces.txt");
        
        fout << length << std::endl;
        for (int i = 0; i < length; i++) {
            fout << data[i].name << " ";
            fout << data[i].countPVZ << " ";
            fout << data[i].city << " ";
            fout << data[i].price << " ";
            fout << data[i].typesInfo.marketPlaceType << " ";
            fout << data[i].typesInfo.businessType << std::endl;
        }
        fout.close();
    }
    void loadFromText() {
        std::ifstream fin("marketplaces.txt");  
        fin >> length;
        fin.ignore(10000, '\n');

        data = new MarketPlace[length];

        for (int i = 0; i < length; i++) {
            int mpTypeNum = 0; 
            int bTypeNum = 0;    

            fin >> data[i].name
                >> data[i].countPVZ
                >> data[i].city
                >> data[i].price
                >> mpTypeNum >> bTypeNum;

            switch (mpTypeNum) {
            case 0:  data[i].typesInfo.marketPlaceType = grocery; break;
            case 1:  data[i].typesInfo.marketPlaceType = homeGoods; break;
            case 2:  data[i].typesInfo.marketPlaceType = stuff; break;
            }

            switch (bTypeNum) {
            case 0:  data[i].typesInfo.businessType = primary; break;
            case 1:  data[i].typesInfo.businessType = auxiliary; break;
            case 2:  data[i].typesInfo.businessType = franchise; break;
            }
        }

        fin.close();
    }
    void saveToBinary() {
        std::ofstream fout("marketplaces.bin", std::ios::binary);


        fout.write((char*)&length, sizeof(length));

        for (int i = 0; i < length; i++) {
            const MarketPlace& item = data[i];

            size_t nameLen = item.name.size();
            fout.write((char*)&nameLen, sizeof(nameLen));
            fout.write(item.name.data(), nameLen);

            fout.write((char*)&item.countPVZ, sizeof(item.countPVZ));

            size_t cityLen = item.city.size();
            fout.write((char*)&cityLen, sizeof(cityLen));
            fout.write(item.city.data(), cityLen);

            fout.write((char*)&item.price, sizeof(item.price));

            fout.write((char*)&item.typesInfo.marketPlaceType, sizeof(item.typesInfo.marketPlaceType));
            fout.write((char*)&item.typesInfo.businessType, sizeof(item.typesInfo.businessType));
        }

        fout.close();
    }

    void loadFromBinary() {
        std::ifstream fin("marketplaces.bin", std::ios::binary);

        if (data != nullptr) {
            delete[] data;
            data = nullptr;
        }

        fin.read((char*)&length, sizeof(length));

        if (length == 0) {
            fin.close();
            return;
        }

        data = new MarketPlace[length];

        for (int i = 0; i < length; i++) {
            size_t len = 0;

            fin.read((char*)&len, sizeof(len));
            data[i].name.resize(len);
            fin.read(&data[i].name[0], len);

            fin.read((char*)&data[i].countPVZ, sizeof(data[i].countPVZ));

         
            fin.read((char*)&len, sizeof(len));
            data[i].city.resize(len);
            fin.read(&data[i].city[0], len);

            fin.read((char*)&data[i].price, sizeof(data[i].price));

            int mpType = 0, bType = 0;
            fin.read((char*)&mpType, sizeof(mpType));
            fin.read((char*)&bType, sizeof(bType));

            data[i].typesInfo.marketPlaceType = static_cast<MarketPlaceType>(mpType);
            data[i].typesInfo.businessType = static_cast<BusinessType>(bType);
        }

        fin.close();
    }

};




int main()
{
    setlocale(LC_ALL, "ru");
    MarketArray marketPlaces;

    marketPlaces.add({ "Ozon", 30000, "Москва", 5000.0, {stuff, primary} });
    marketPlaces.add({ "Wildberries", 35000, "Москва", 7500.0, {stuff, primary} });
    marketPlaces.add({ "Samokat", 1200, "Питер", 850.0, {grocery, primary} });
    marketPlaces.add({ "KazanExpress", 550, "Казань", 400.0, {stuff, franchise} });
    marketPlaces.add({ "VkusVill", 1500, "Москва", 1300.0, {grocery, primary} });
    marketPlaces.add({ "Yandex_Market", 18000, "Москва", 4200.0, {stuff, primary} });
    marketPlaces.add({ "SberMarket", 2500, "Москва", 1600.0, {grocery, auxiliary} });
    marketPlaces.add({ "Lamoda", 900, "Москва", 1150.0, {stuff, primary} });
    marketPlaces.add({ "AliExpress", 11000, "Ханчжоу", 9500.0, {stuff, auxiliary} });
    marketPlaces.add({ "Leroy_Merlin", 160, "Париж", 2100.0, {homeGoods, primary} });
    marketPlaces.add({ "Magnit", 4500, "Краснодар", 950.0, {grocery, auxiliary} });
    marketPlaces.add({ "Auchan", 350, "Лилль", 650.0, {grocery, auxiliary} });
    marketPlaces.add({ "IKEA", 45, "Делфт", 1550.0, {homeGoods, primary} });
    marketPlaces.add({ "Lenta", 480, "Питер", 420.0, {grocery, auxiliary} });
    marketPlaces.add({ "Perekrestok", 720, "Москва", 750.0, {grocery, primary} });
    marketPlaces.add({ "Metro", 110, "Мюнхен", 520.0, {grocery, franchise} });
    marketPlaces.add({ "Hoff", 65, "Москва", 310.0, {homeGoods, primary} });
    marketPlaces.add({ "Sokolov", 420, "Кострома", 220.0, {stuff, franchise} });
    marketPlaces.add({ "FixPrice", 5200, "Химки", 1850.0, {homeGoods, primary} });
    marketPlaces.add({ "Petrovich", 210, "Питер", 460.0, {homeGoods, primary} });


    marketPlaces.print();
    std::cout << std::endl << std::endl;

    MarketArray marketPlacesGrocery;
    marketPlaces.copyByMarketPlaceTypeTo(marketPlacesGrocery, grocery);
    marketPlacesGrocery.print();
    std::cout << std::endl << std::endl;

    marketPlacesGrocery.sortByCountPVZ();
    marketPlacesGrocery.print();
    std::cout << std::endl << std::endl;

    marketPlaces.print("Ozon");
    std::cout << std::endl << std::endl;

    marketPlaces.printTop3();
    std::cout << std::endl << std::endl;

    marketPlaces.printNonKazan();
    std::cout << std::endl << std::endl;


    marketPlaces.data[0].updateMarketPlace(3000, 1800.0, grocery, primary);
    marketPlaces.print();

    std::cout << std::endl << std::endl << "Файлы" << std::endl << std::endl;


    marketPlaces.saveToText();
    marketPlaces.saveToBinary();

    delete[] marketPlaces.data;
    marketPlaces.data = nullptr;
    marketPlaces.length = 0;


    marketPlaces.loadFromText();
    marketPlaces.print();

    std::cout << std::endl << std::endl << "binary" << std::endl << std::endl;

    marketPlaces.loadFromBinary();
    marketPlaces.print();


    return 0;
}

