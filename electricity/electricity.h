﻿#pragma once
#include <string>

class Object;

/// <summary>
/// Полюс устройства.
/// К одному полюсу может быть подключен один любой другой полюс этого же
/// устройства, или же любой полюс любого другого устройства.
/// 
/// У каждого полюса есть своё название, которое получает значение по умолчанию
/// при создании устройства, но может быть изменено позднее.
/// </summary>
struct Pole {
    /// <summary>
    /// Название полюса.
    /// Должно быть уникальным в пределах устройства.
    /// </summary>
    std::string name;

    /// <summary>
    /// Устройство, подключенное к данному полюсу.
    /// Может совпадать с устройством, содержащим данный полюс, при условии, что соединяются
    /// два разных полюса.
    /// Значение <c>nullptr</c> означает, что к данному полюсу ничего не подключено.
    /// </summary>
    Object* connectedObject;

    /// <summary>
    /// Полюс устройства, к которому подключен данный полюс.
    /// </summary>
    std::string connectedObjectPole;

    Pole(const std::string& name_) : name(name_), connectedObject(nullptr) {}
};

/// <summary>
/// Электротехническое устройство.
/// Имеет произвольное неотрицательное количество полюсов (<see cref="Pole"/>),
/// через которые может соединяться с другими устройствами.
/// </summary>
class Object {
    std::string name;
protected:
    Object(const std::string& name_) : name(name_) {}

    /// <summary>
    /// Возвращает полюс по внутреннему индексу устройства.
    /// </summary>
    /// <param name="idx">Индекс полюса, от <c>0</c> до значения, возвращаемого <see cref="getPoleCount()"/>.</param>
    /// <returns>Полюс с указанным индексом, или <c>nullptr</c>, если такой полюс не существует.</returns>
    Pole* getPole(size_t idx) { return const_cast<Pole*>(const_cast<const Object*>(this)->getPole(idx)); }

    /// <summary>
    /// Возвращает полюс по внутреннему индексу устройства.
    /// </summary>
    /// <param name="idx">Индекс полюса, от <c>0</c> до значения, возвращаемого <see cref="getPoleCount()"/>.</param>
    /// <returns>Полюс с указанным индексом, или <c>nullptr</c>, если такой полюс не существует.</returns>
    virtual const Pole* getPole(size_t idx) const = 0;

public:
    virtual ~Object() {}

    const std::string& getName() const { return name; }

    void getName(const std::string &newName) { name = newName; }

    /// <summary>
    /// Возвращает полюс по имени.
    /// </summary>
    /// <param name="name">Имя полюса.</param>
    /// <returns>Полюс с указанным именем, или <c>nullptr</c>, если такой полюс не существует.</returns>
    Pole* getPole(const std::string& name) { return const_cast<Pole*>(const_cast<const Object*>(this)->getPole(name)); }

    /// <summary>
    /// Возвращает полюс по имени.
    /// </summary>
    /// <param name="name">Имя полюса.</param>
    /// <returns>Полюс с указанным именем, или <c>nullptr</c>, если такой полюс не существует.</returns>
    virtual const Pole* getPole(const std::string& name) const = 0 ;

    /// <returns>Количество полюсов в текущем устройстве.</returns>
    virtual size_t getPoleCount() const = 0;

    /// <summary>
    /// Проверяет, связаны ли два указанных устройства напрямую через какую-либо пару их полюсов.
    /// </summary>
    /// <param name="other">Устройство, наличие прямой связи с которым проверяется.</param>
    /// <returns><c>true</c> если устройства связаны напрямую, <c>false</c> в противном случае.</returns>
    /// 
    bool isConnectedTo(const Object& other) const {
        for (int i = 1;i <= getPoleCount();i++) {
            for (int j = 1;j <= other.getPoleCount();j++) {
                if (getPole(i)->connectedObjectPole == other.getPole(j)->name  &&  getPole(i)->connectedObject == const_cast<Object*>(&other)) // проверим что полюса совпадают; имена совпадают 
                    return true;
            }
        }
        return false;
    }
    

    /// <summary>
    /// Соединяет указанные полюса текущего и указанного устройства.
    /// Если к этим полюсам было что-то подключено, то такая связь разрушается.
    /// </summary>
    /// <param name="poleName">Название подключаемого полюса текущего устройства</param>
    /// <param name="other">Устройство, которое связывается с текущим.</param>
    /// <param name="otherPoleName">Название подключаемого полюса другого устройства</param>
    /// <remarks>
    /// Может использоваться для связи устройства с самим собой.
    /// В этом случае в качестве <paramref name="other"/> следует передать то же устройство,
    /// для которого вызывается этот метод.
    /// </remarks>
    bool connect(const std::string& poleName, Object& other, const std::string& otherPoleName) {
        if (poleName != otherPoleName) {
            Pole* p1 = getPole(poleName);
            Pole* p2 = other.getPole(otherPoleName);
            p1->connectedObject = const_cast<Object*>(&other); // тк это указатель поле, а у на спередатется ссылка
            p1->connectedObjectPole = otherPoleName;
            p2->connectedObjectPole = poleName;
            p2->connectedObject = this;    // const_cast<Object*>
            return true;
        }
        return false;
    }
    bool disconnect(const std::string& poleName) { // отключаем  конкретные полюса
        if(getPole(poleName)->connectedObject != nullptr) {
            //отключаем у другого прибора, потом у себя
            Pole* p = getPole(poleName);
            getPole(p->connectedObjectPole)->connectedObject = nullptr; 
            getPole(p->connectedObjectPole)->connectedObjectPole = "";
            p->connectedObject = nullptr;
            p->connectedObjectPole = "";

            return true;
        }
        return false;
    }
    
};


/// <summary>
/// Простой выключатель с двумя полюсами.
/// </summary>
class Switch : public Object {
    std::string name;
public:
    Pole a1, a2;

    Switch(const std::string& name = " ") : Object(name), a1("A1"), a2("A2") {}
    

    size_t getPoleCount() const { return 2; }

    const Pole* getPole(const std::string& name) const {
        if (name == a1.name)
            return &a1;
        if (name == a2.name)
            return &a2;
        return nullptr;
    }


protected:
    const Pole* getPole(size_t idx) const {
        if (idx == 1)
            return &a1;
        if (idx == 2)
            return &a2;
        return nullptr;
    }
};





// TODO: класс светильника с двумя полюсами
class Light : public Object {
    std::string name;
public:
    Pole l1, l2;
    Light(const std::string& name = " "):  Object(name), l1("A1"), l2("A2") {}

    size_t getPoleCount() const { return 2; }

    const Pole* getPole(const std::string& name) const {
        if (name == l1.name)
            return &l1;
        if (name == l2.name)
            return &l2;
        return nullptr;
    }

protected:
    const Pole* getPole(size_t idx) const {
        if (idx == 1)
            return &l1;
        if (idx == 2)
            return &l2;
        return nullptr;
    }

};




// TODO: класс генератора с тремя полюсами (фаза, нейтраль, земпя).
class Generator : public Object {
    std::string name;
protected:
    const Pole* getPole(size_t idx) const {
        if (idx == 1)
            return &a1;
        if (idx == 2)
            return &a2;
        if (idx == 3)
            return &a3;
        return nullptr;
    }
public:
    Pole a1, a2, a3;

    Generator(const std::string& name = " ") : Object(name), a1("A1"), a2("A2"),a3("A3") {}

    size_t getPoleCount() const { return 3; }

    const Pole* getPole(const std::string& name) const {
        if (name == a1.name)
            return &a1;
        if (name == a2.name)
            return &a2;
        if (name == a3.name)
            return &a3;
        return nullptr;
    }
};