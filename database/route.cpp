#include "route.h"
#include "database.h"
#include "../config/config.h"

#include <Poco/Data/MySQL/Connector.h>
#include <Poco/Data/MySQL/MySQLException.h>
#include <Poco/Data/SessionFactory.h>
#include <Poco/Data/RecordSet.h>
#include <Poco/JSON/Parser.h>
#include <Poco/Dynamic/Var.h>

#include <sstream>
#include <exception>

using namespace Poco::Data::Keywords;
using Poco::Data::Session;
using Poco::Data::Statement;

namespace database
{

    void Route::init()
    {
        try
        {
            Poco::Data::Session session = database::Database::get().create_session();
            Statement create_stmt(session);
            create_stmt << "CREATE TABLE IF NOT EXISTS `route` (`id` INT NOT NULL AUTO_INCREMENT,"
                        << "`name` VARCHAR(256) NOT NULL,"
                        << "`starting_point` VARCHAR(256) NOT NULL,"
                        << "`end_point` VARCHAR(256) NOT NULL,"
                        << "`description` TEXT NOT NULL,"
                        << "`date` VARCHAR(256) NOT NULL,"
                        << "`author_id` INT NOT NULL,"
                        << "PRIMARY KEY (`id`),KEY `fn` (`author_id`));",
                now;
        }

        catch (Poco::Data::MySQL::ConnectionException &e)
        {
            std::cout << "connection:" << e.what() << std::endl;
            throw;
        }
        catch (Poco::Data::MySQL::StatementException &e)
        {
            std::cout << "statement:" << e.what() << std::endl;
            throw;
        }
    }

    Poco::JSON::Object::Ptr Route::toJSON() const
    {
        Poco::JSON::Object::Ptr root = new Poco::JSON::Object();

        root->set("id", _id);
        root->set("name", _name);
        root->set("starting_point", _starting_point);
        root->set("end_point", _end_point);
        root->set("description", _description);
        root->set("date", _date);
        root->set("author_id", _author_id);

        return root;
    }

    Route Route::fromJSON(const std::string &str)
    {
        Route route;
        Poco::JSON::Parser parser;
        Poco::Dynamic::Var result = parser.parse(str);
        Poco::JSON::Object::Ptr object = result.extract<Poco::JSON::Object::Ptr>();

        route.id() = object->getValue<long>("id");
        route.name() = object->getValue<std::string>("name");
        route.starting_point() = object->getValue<std::string>("starting_point");
        route.end_point() = object->getValue<std::string>("end_point");
        route.description() = object->getValue<std::string>("description");
        route.date() = object->getValue<std::string>("date");
        route.author_id() = object->getValue<long>("author_id");

        return route;
    }
    
    std::vector<Route> Route::routesearch(std::string author_id)
    {
        try
        {
            Poco::Data::Session session = database::Database::get().create_session();
            Statement select(session);
            std::vector<Route> result;
            User a;
            first_name += "%";
            last_name += "%";
            select << "SELECT name,starting_point,end_point,description,date,author_id FROM Route where author_id LIKE ?",
                into(a._id),
                into(a._name),
                into(a._starting_point),
                into(a._end_point),
                into(a._description),
                into(a._date),
                into(a._author_id),
                use(author_id),
                range(0, 1); //  iterate over result set one row at a time

            while (!select.done())
            {
                if (select.execute())
                    result.push_back(a);
            }
            return result;
        }

        catch (Poco::Data::MySQL::ConnectionException &e)
        {
            std::cout << "connection:" << e.what() << std::endl;
            throw;
        }
        catch (Poco::Data::MySQL::StatementException &e)
        {

            std::cout << "statement:" << e.what() << std::endl;
            throw;
        }
    }

    void Route::save_to_mysql()
    {

        try
        {
            Poco::Data::Session session = database::Database::get().create_session();
            Poco::Data::Statement insert(session);

            insert << "INSERT INTO Route (name,starting_point,end_point,description,date,author_id) VALUES(?, ?, ?, ?, ?, ?)",
                use(_name),
                use(_starting_point),
                use(_end_point),
                use(_description),
                use(_date),
                use(_author_id);

            insert.execute();

            Poco::Data::Statement select(session);
            select << "SELECT LAST_INSERT_ID()",
                into(_id),
                range(0, 1); //  iterate over result set one row at a time

            if (!select.done())
            {
                select.execute();
            }
            std::cout << "inserted:" << _id << std::endl;
        }
        catch (Poco::Data::MySQL::ConnectionException &e)
        {
            std::cout << "connection:" << e.what() << std::endl;
            throw;
        }
        catch (Poco::Data::MySQL::StatementException &e)
        {

            std::cout << "statement:" << e.what() << std::endl;
            throw;
        }
    }
    
    const std::string &route::get_name() const
    {
        return _name;
    }

    const std::string &route::get_starting_point() const
    {
        return _starting_point;
    }

    std::string &route::name()
    {
        return _name;
    }

    long route::get_id() const
    {
        return _id;
    }

    const std::string &route::get_starting_point() const
    {
        return _starting_point;
    }

    const std::string &route::get_end_point() const
    {
        return _end_point;
    }

    const std::string &route::get_author_id() const
    {
        return _author_id;
    }

    const std::string &route::get_description() const
    {
        return _description;
    }

    const std::string &route::get_date() const
    {
        return _date;
    }

    long &route::id()
    {
        return _id;
    }

    std::string &route::starting_point()
    {
        return _starting_point;
    }

    std::string &route::end_point()
    {
        return _end_point;
    }

    std::string &route::description()
    {
        return _description;
    }

    std::string &route::date()
    {
        return _date;
    }

    std::string &route::author_id()
    {
        return _author_id;
    }
}