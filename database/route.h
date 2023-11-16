#ifndef AUTHOR_H
#define AUTHOR_H

#include <string>
#include <vector>
#include "Poco/JSON/Object.h"
#include <optional>

namespace database
{
    class Route{
        private:
            long _id;
            std::string _name;
            std::string _starting_point;
            std::string _end_point;
            std::string _description;
            std::string _date;
            std::string _author_id;

        public:

            static Route fromJSON(const std::string & str);

            long               get_id() const;
            const std::string &get_name() const;
            const std::string &get_starting_point() const;
            const std::string &get_end_point() const;
            const std::string &get_description() const;
            const std::string &get_date() const;
            const std::string &get_author_id() const;

            long&        id();
            std::string &name();
            std::string &starting_point();
            std::string &end_point();
            std::string &description();
            std::string &date();
            std::string &author_id();

            static void init();
            static std::vector<Route> routesearch(std::string author_id);
            void save_to_mysql();

            Poco::JSON::Object::Ptr toJSON() const;

    };
}

#endif