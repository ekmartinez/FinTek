import sqlite3

def create_database(db_name="ledger.db"):
    # Connect to SQLite (creates the file if it doesn't exist)
    conn = sqlite3.connect(db_name)
    cursor = conn.cursor()

    # Enforce foreign key constraints
    cursor.execute("PRAGMA foreign_keys = ON;")

    # Create Category table
    cursor.execute("""
        CREATE TABLE IF NOT EXISTS Category (
            CategoryId INTEGER PRIMARY KEY AUTOINCREMENT,
            CategoryDescription TEXT UNIQUE NOT NULL
        );
    """)

    # Create Transactions table
    cursor.execute("""
        CREATE TABLE IF NOT EXISTS Transactions (
            TransactionId INTEGER PRIMARY KEY AUTOINCREMENT,
            Date TEXT NOT NULL CHECK (
                Date GLOB '[0-9][0-9][0-9][0-9]-[0-1][0-9]-[0-3][0-9]'
            ),
            Description TEXT,
            CategoryId INTEGER NOT NULL,
            Amount REAL NOT NULL CHECK (Amount <> 0),
            Type TEXT CHECK (Type IN ('income', 'expense')) NOT NULL,
            CreatedAt TEXT DEFAULT (datetime('now')),
            FOREIGN KEY (CategoryId) REFERENCES Category(CategoryId)
        );
    """)

    # Create useful indexes
    cursor.execute("CREATE INDEX IF NOT EXISTS idx_transactions_date ON Transactions(Date);")
    cursor.execute("CREATE INDEX IF NOT EXISTS idx_transactions_category ON Transactions(CategoryId);")

    # Commit and close
    conn.commit()
    conn.close()
    print(f"Database '{db_name}' created successfully with all tables.")

if __name__ == "__main__":
    create_database()
