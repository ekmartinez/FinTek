#ifndef QUERIES_H
#define QUERIES_H

// ============================================================================
//  CREATE (Insert)
// ============================================================================
static const char* SQL_INSERT_TRANSACTION = R"(
    INSERT INTO Transactions (Date, Description, CategoryId, Amount, Type)
    VALUES (?, ?, ?, ?, ?);
)";

static const char* SQL_INSERT_CATEGORY = R"(
    INSERT INTO Category (CategoryDescription)
    VALUES (?);
)";

// ============================================================================
//  READ (Select)
// ============================================================================
static const char* SQL_GET_ALL = R"(
    SELECT
        t.TransactionId,
        t.Date,
        t.Description,
        t.CategoryId,
        c.CategoryDescription,
        t.Amount,
        t.Type
    FROM Transactions t
    LEFT JOIN Category c ON t.CategoryId = c.CategoryId
    ORDER BY t.Date DESC;
)";

static const char* SQL_GET_BY_ID = R"(
    SELECT
        t.TransactionId,
        t.Date,
        t.Description,
        c.CategoryDescription,
        t.Amount,
        t.Type
    FROM Transactions t
    LEFT JOIN Category c ON t.CategoryId = c.CategoryId
    WHERE t.TransactionId = ?;
)";

static const char* SQL_GET_BY_CATEGORY = R"(
    SELECT
        t.TransactionId,
        t.Date,
        t.Description,
        c.CategoryDescription,
        t.Amount,
        t.Type
    FROM Transactions t
    LEFT JOIN Category c ON t.CategoryId = c.CategoryId
    WHERE c.CategoryDescription = ?;
)";

static const char* SQL_GET_BY_DATERANGE = R"(
    SELECT
        t.TransactionId,
        t.Date,
        t.Description,
        c.CategoryDescription,
        t.Amount,
        t.Type
    FROM Transactions t
    LEFT JOIN Category c ON t.CategoryId = c.CategoryId
    WHERE t.Date BETWEEN ? AND ?
    ORDER BY t.Date ASC;
)";

// ============================================================================
//  UPDATE
// ============================================================================
static const char* SQL_UPDATE_TRANSACTION = R"(
    UPDATE Transactions
    SET Date = ?, Description = ?, CategoryId = ?, Amount = ?, Type = ?
    WHERE TransactionId = ?;
)";

static const char* SQL_UPDATE_CATEGORY = R"(
    UPDATE Category
    SET CategoryDescription = ?
    WHERE CategoryId = ?;
)";

// ============================================================================
//  DELETE
// ============================================================================
static const char* SQL_DELETE_TRANSACTION = R"(
    DELETE FROM Transactions
    WHERE TransactionId = ?;
)";

static const char* SQL_DELETE_CATEGORY = R"(
    DELETE FROM Category
    WHERE CategoryId = ?;
)";

static const char *SQL_GET_CATEGORY_ID = R"(
    SELECT CategoryId
    FROM Category
    WHERE CategoryDescription = ?;
)";

#endif // QUERIES_H
