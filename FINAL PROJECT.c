#include <stdio.h>
#include <string.h>
#include <time.h>

#define MAX_ITEMS 10
#define TAX_RATE 0.18
#define USERNAME "Saroksha"
#define PASSWORD "141514"
#define MAX_CUSTOMERS 100

int validateUser(char *username, char *password);

struct MenuItem
{
    char name[30];
    float price;
};

struct Order
{
    char customerName[50];
    char itemNames[MAX_ITEMS][30];
    int quantities[MAX_ITEMS];
    float itemTotals[MAX_ITEMS];
    int itemCount;
    float totalAmount;
};

void dispMenu(struct MenuItem menu[], int size)
{
    printf("\n Menu-\n");
    for (int i = 0; i < size; i++) {
        printf("%d. %s - Rs %.2f\n", i + 1, menu[i].name, menu[i].price);
    }
    printf("\n");
}

float procOrder(struct MenuItem menu[], int size, int orderCount[], struct Order *order)
{
    char orderName[30];
    int quantity;
    float tamt = 0.0;
    order->itemCount = 0;

    printf("Enter 'done' when you are finished ordering.\n");

    while (1)
    {
        printf("\nEnter item name: ");
        fgets(orderName, sizeof(orderName), stdin);
        orderName[strcspn(orderName, "\n")] = '\0';

        if (strcmp(orderName, "done") == 0)
        {
            break;
        }

        int found = 0;
        for (int i = 0; i < size; i++)
        {
            if (strcmp(menu[i].name, orderName) == 0)
            {
                printf("Enter quantity for %s: ", orderName);
                scanf("%d", &quantity);
                getchar();

                tamt += menu[i].price * quantity;
                order->itemTotals[order->itemCount] = menu[i].price * quantity;
                strcpy(order->itemNames[order->itemCount], orderName);
                order->quantities[order->itemCount] = quantity;
                order->itemCount++;
                orderCount[i] += quantity;
                printf("Added %d x %s to your order. Total: Rs %.2f\n", quantity, orderName, menu[i].price * quantity);
                found = 1;
                break;
            }
        }

        if (!found)
        {
            printf("Item not found. Please select a valid menu item.\n");
        }
    }

    order->totalAmount = tamt;
    return tamt;
}

void applyDisc(float *tamt)
{
    if (*tamt >= 500)
    {
        *tamt -= *tamt * 0.15;
        printf("Discount applied! 15%% off for exceeding Rs.500.\n");
    }
    else if (*tamt >= 300)
    {
        *tamt -= *tamt * 0.10;
        printf("Discount applied! 10%% off for exceeding Rs.300.\n");
    }
    else if (*tamt >= 200)
    {
        *tamt -= *tamt * 0.05;
        printf("Discount applied! 5%% off for exceeding Rs.200.\n");
    }
}

void applyTax(float *tamt)
{
    *tamt += *tamt * TAX_RATE;
}

int validateUser(char *username, char *password)
{
    if (strcmp(username, USERNAME) == 0 && strcmp(password, PASSWORD) == 0)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

void processPayment(float total)
{
    int paymentChoice;
    printf("\nChoose Payment Mode:\n");
    printf("1. Cash\n");
    printf("2. Credit Card\n");
    printf("3. Debit Card\n");
    printf("4. UPI\n");
    printf("Enter your choice: ");
    scanf("%d", &paymentChoice);

    switch (paymentChoice)
    {
        case 1:
            printf("You have chosen to pay by Cash. Please pay Rs %.2f.\n", total);
            printf("Payment received in cash. Thank you!\n");
            break;
        case 2:
            printf("You have chosen to pay by Credit Card. Please swipe your card for Rs %.2f.\n", total);
            printf("Payment successful via Credit Card. Thank you!\n");
            break;
        case 3:
            printf("You have chosen to pay by Debit Card. Please swipe your card for Rs %.2f.\n", total);
            printf("Payment successful via Debit Card. Thank you!\n");
            break;
        case 4:
            printf("You have chosen to pay via UPI. Please scan the UPI QR code to pay Rs %.2f.\n", total);
            printf("Payment successful via UPI. Thank you!\n");
            break;
        default:
            printf("Invalid choice. Payment canceled.\n");
            break;
    }
}

void displayOrderSummary(int orderCount[], struct MenuItem menu[], int size)
{
    printf("\nOrder Summary for the Day:\n");
    printf("Item Description             Total Orders\n");
    for (int i = 0; i < size; i++)
    {
        if (orderCount[i] > 0)
        {
            printf("%-25s %d\n", menu[i].name, orderCount[i]);
        }
    }
}

void displayAllOrders(struct Order orders[], int orderCount)
{
    printf("\nAll Orders for the Day:\n");
    for (int i = 0; i < orderCount; i++)
    {
        printf("Customer Name: %s\n", orders[i].customerName);
        printf("Items Ordered:\n");
        for (int j = 0; j < orders[i].itemCount; j++)
        {
            printf("%s - %d x Rs %.2f = Rs %.2f\n", orders[i].itemNames[j], orders[i].quantities[j], orders[i].itemTotals[j] / orders[i].quantities[j], orders[i].itemTotals[j]);
        }
        printf("Total Amount: Rs %.2f\n", orders[i].totalAmount);
        printf("----------------------------\n");
    }
}

void saveOrderToCSV(struct Order *order)
{
    FILE *file = fopen("PROJECT.csv", "a");
    if (file == NULL)
    {
        printf("Could not open file for writing.\n");
        return;
    }
if (ftell(file) == 0) {
        fprintf(file, "Customer Name,Item Name1,Quantity1,Item price,Item Name2,Quantity2,Item price,Total Amount\n");
    }

    fprintf(file, "%s,", order->customerName);
    for (int i = 0; i < order->itemCount; i++)
    {
        fprintf(file, "%s,%d,%.2f,", order->itemNames[i], order->quantities[i], order->itemTotals[i]);
    }
    fprintf(file, "%.2f\n", order->totalAmount);

    fclose(file);
}

int main()
{
    char username[50], password[50], customerName[50];
    struct Order orders[MAX_CUSTOMERS];
    int orderCount[MAX_ITEMS] = {0};
    int totalOrders = 0;

    printf("Welcome to the Bengaluru Tiffins\n");

    printf("Enter username: ");
    scanf("%s", username);

    printf("Enter password: ");
    scanf("%s", password);

    if (validateUser(username, password))
    {
        printf("\nAccess granted!\n");
        printf("You can now use the billing system.\n");

        struct MenuItem menu[] =
        {
            {"Masala Dosa", 70.0},
            {"Idli", 40.0},
            {"Pongal", 45.0},
            {"Khara Bath", 30.0},
            {"Kesari Bath", 40.0},
            {"Vada", 30.0},
            {"Coffee", 25.0},
            {"Tea", 20.0}
        };

        int menuSize = sizeof(menu) / sizeof(menu[0]);
        float tamt = 0.0;
        int choice;

        while (1)
        {
            printf("\n Bengaluru Tiffins-\n");
            printf("1. Display Menu\n");
            printf("2. Place Order\n");
            printf("3. Calculate Total & Apply Discount\n");
            printf("4. View Order Summary\n");
            printf("5. View All Orders for the Day\n");
            printf("6. Exit\n");
            printf("Enter your choice: ");
            scanf("%d", &choice);
            getchar();

            switch (choice)
            {
                case 1:
                    dispMenu(menu, menuSize);
                    break;
                case 2:
                    printf("Enter customer name: ");
                    fgets(customerName, sizeof(customerName), stdin);
                    customerName[strcspn(customerName, "\n")] = '\0';

                    tamt = procOrder(menu, menuSize, orderCount, &orders[totalOrders]);
                    strcpy(orders[totalOrders].customerName, customerName);

                    applyDisc(&tamt);
                    applyTax(&tamt);
                    orders[totalOrders].totalAmount = tamt;

                    totalOrders++;

                    printf("\nFinal Bill for %s-\n", customerName);
                    printf("Sl. No.  Item Description       Quantity  Unit Price  Total Price\n");
                    for (int i = 0; i < orders[totalOrders - 1].itemCount; i++)
                    {
                        printf("%-8d %-20s %-9d Rs %.2f  Rs %.2f\n", i + 1, orders[totalOrders - 1].itemNames[i], orders[totalOrders - 1].quantities[i], menu[i].price, orders[totalOrders - 1].itemTotals[i]);
                    }
                    printf("------------------------------------------------------------\n");
                    printf("Total Amount: Rs %.2f\n", orders[totalOrders - 1].totalAmount);

                    char confirm;
                    printf("Do you confirm this bill? (y/n): ");
                    scanf(" %c", &confirm);
                    if (confirm == 'y' || confirm == 'Y')
                    {
                        processPayment(tamt);
                        saveOrderToCSV(&orders[totalOrders - 1]);  // Save order to CSV
                    }
                    else
                    {
                        printf("Payment canceled. You can modify your order.\n");
                    }
                    break;
                case 3:
                    applyDisc(&tamt);
                    applyTax(&tamt);
                    printf("\nFinal amount after all discounts and tax: Rs %.2f\n", tamt);
                    break;
                case 4:
                    displayOrderSummary(orderCount, menu, menuSize);
                    break;
                case 5:
                    displayAllOrders(orders, totalOrders);
                    break;
                case 6:
                    printf("Thank you for visiting our restaurant!\n");
                    return 0;
                default:
                    printf("Invalid choice! Please enter a valid option.\n");
            }
        }
    }
    else
    {
        printf("\nAccess denied! Invalid credentials.\n");
    }

    return 0;
}
