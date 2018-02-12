#include "Database.h"

Database::Database()
{
}

Database::~Database()
{
}

void Database::addTable(const std::string tableName, Table<Resource>* table)
{
	tables[tableName] = table;
}

Table<Resource>* Database::getTable(const std::string tableName)
{
	return tables.at(tableName);
}

void Database::addResourceToTable(const std::string tableName, Resource* resource)
{
	tables.at(tableName)->addNewResource(resource);
}

void Database::addResourceToTable(const std::string tableName, Node* node)
{
	tables.at(tableName)->addNewResource(node);
}