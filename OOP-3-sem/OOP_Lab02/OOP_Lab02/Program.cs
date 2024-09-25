using OOP_Lab02;

class Program
{
    static void Main()
    {
        Airline airline1 = new();
        Airline airline2 = new("London, UK", 123, "Airbus A320", new TimeOnly(15, 30), [DayOfWeek.Tuesday, DayOfWeek.Friday]);
        Airline airline3 = airline2;

        Console.WriteLine(airline1);
        Console.WriteLine();
        Console.WriteLine(airline2);

        int flightNumber = airline1.FlightNumber;
        Airline.UpdateFlightNumber(ref flightNumber, out DateTime updatedDepartureTime);
        Console.WriteLine();
        Console.WriteLine($"Обновленный номер рейса: {flightNumber}, Время отправки: {updatedDepartureTime}");

        Airline.DisplayObjectCount();

        Console.WriteLine();
        Console.WriteLine(airline1.GetHashCode());
        Console.WriteLine(airline1.GetType());

        Console.WriteLine();
        Console.WriteLine(airline3.Equals(airline2));
        Console.WriteLine(airline3 == airline2);

        Airline[] airlines =
    [
            new Airline("London, UK", 101, "Airbus A320", new TimeOnly(10, 30, 0), [DayOfWeek.Tuesday, DayOfWeek.Friday]),
            new Airline("New York, USA", 102, "Boeing 747", new TimeOnly(14, 15, 0), [DayOfWeek.Wednesday, DayOfWeek.Saturday]),
            new Airline("Tokyo, Japan", 103, "Airbus A380", new TimeOnly(8, 45, 0), [DayOfWeek.Monday, DayOfWeek.Thursday]),
            new Airline("Paris, France", 104, "Boeing 787", new TimeOnly(16, 0, 0), [DayOfWeek.Sunday, DayOfWeek.Thursday]),
            new Airline("Berlin, Germany", 105, "Embraer E190", new TimeOnly(12, 30, 0), [DayOfWeek.Monday, DayOfWeek.Friday]),
            new Airline("Sydney, Australia", 106, "Boeing 777", new TimeOnly(18, 0, 0), [DayOfWeek.Wednesday, DayOfWeek.Saturday]),
            new Airline("Moscow, Russia", 107, "Sukhoi Superjet 100", new TimeOnly(13, 15, 0), [DayOfWeek.Tuesday, DayOfWeek.Friday]),
            new Airline("Dubai, UAE", 108, "Airbus A350", new TimeOnly(20, 45, 0), [DayOfWeek.Thursday]),
            new Airline("Toronto, Canada", 109, "Boeing 737", new TimeOnly(9, 10, 0), [DayOfWeek.Monday, DayOfWeek.Friday]),
            new Airline("Toronto, Canada", 110, "Boeing 787", new TimeOnly(7, 0, 0), [DayOfWeek.Wednesday, DayOfWeek.Saturday])
    ];

        string dest = "Toronto, Canada";
        DayOfWeek dayOfWeek = DayOfWeek.Monday;

        List<Airline> foundByDests = [];
        List<Airline> foundByDays = [];

        foreach (var airline in airlines)
        {
            if (airline.Destination == dest)
            {
                foundByDests.Add(airline);
            }
            if (airline.DaysOfWeeks.Contains(dayOfWeek))
            {
                foundByDays.Add(airline);
            }
        }

        void PrintList(List<Airline> airlines)
        {
            Console.WriteLine();
            foreach (var airline in airlines)
            {
                Console.WriteLine(airline);
            }
        }

        PrintList(foundByDests);
        PrintList(foundByDays);

        var flightInfo = new
        {
            Destination = "London, UK",
            FlightNumber = 101,
            PlaneType = "Airbus A320",
            DepartureTime = new TimeSpan(10, 30, 0),
            DaysOfWeeks = new[] { DayOfWeek.Tuesday, DayOfWeek.Friday }
        };

        Console.WriteLine($"\nместо назначения: {flightInfo.Destination}");
        Console.WriteLine($"номер полета: {flightInfo.FlightNumber}");
        Console.WriteLine($"самолет: {flightInfo.PlaneType}");
        Console.WriteLine($"время отправления: {flightInfo.DepartureTime}");
        Console.WriteLine($"Дни: {string.Join(", ", flightInfo.DaysOfWeeks)}");
        Console.WriteLine(flightInfo.ToString());
        Console.WriteLine();
    }
}