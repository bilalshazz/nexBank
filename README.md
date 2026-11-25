# nexBank 🏦

A command line banking application written in C. It lets users create an account, log in, manage their balance, send money to other users and estimate their UK income tax.

## What it does

* **Sign up** with a username and password, and receive a unique six digit userID
* **Log in** with up to 5 attempts before access is denied
* **Check balance** at any time
* **Deposit** and **withdraw** funds
* **Pay and transfer** money to another account using their userID
* **Tax calculator** for UK income tax (2026/27, England rates)

## How to use it

1. Start the program and choose **1** to log in or **2** to sign up.
2. If you are signing up, you will need:
   * a username (at least 4 characters, no commas)
   * a password (at least 8 characters, no commas)
3. Once your account is created, you will be taken to the login screen.
4. After logging in, you will see the bank menu:

```
1. Check Balance
2. Deposit Money
3. Withdraw Money
4. Pay & Transfer
5. Tax Calculator
6. Exit
```

To try out transfers, create two accounts and use the second account's userID when paying from the first.

## Try it out

The repository includes `nexBank.csv` and `nexBalance.csv`, which already contain some dummy accounts. You can log in using any of the details in `nexBank.csv`, or sign up and create your own account.

All of the details provided are made up, so no real passwords or personal information are stored anywhere in this project.
