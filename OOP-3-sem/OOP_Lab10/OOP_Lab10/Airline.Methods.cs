namespace OOP_Lab02
{
    public partial class Airline
    {
        static Airline()
        {
            if (objectCount == minInt)
            {
                objectCount = 0;
            }
        }

        private Airline(int id)
        {
            this.id = id;
            objectCount++;
        }

        public Airline() : this("Minsk, Belarus", 1, "Boeing 777", TimeNow, [DayOfWeek.Monday]) { }

        public Airline(string destination, int flightNumber, string planeType, TimeOnly? departureTime = null,
                       DayOfWeek[]? daysOfWeeks = null) : this(GenerateId())
        {
            Destination = destination ?? throw new ArgumentException("destination is null");
            FlightNumber = flightNumber;
            PlaneType = planeType ?? throw new ArgumentException("planeType is null");
            this.departureTime = departureTime ?? TimeNow;
            this.daysOfWeeks = daysOfWeeks ?? [DayOfWeek.Sunday];
        }

        public static void DisplayObjectCount()
        {
            Console.WriteLine($"Всего создано бъектов класса Airline: {objectCount}");
        }

        public static void UpdateFlightNumber(ref int newFlightNumber, out DateTime updatedDepartureTime)
        {
            newFlightNumber = 999;
            updatedDepartureTime = DateTime.Now.AddHours(3);
        }

        public override bool Equals(object? obj)
        {
            if (obj is Airline airline)
            {
                return departureTime == airline.departureTime;
            }

            return false;
        }

        public override int GetHashCode()
        {
            return HashCode.Combine(flightNumber, destination);
        }

        public override string ToString()
        {
            return $"Рейс {flightNumber} в {destination}, {planeType}. Отправка: {departureTime.ToString("HH\\:mm")}, Дни: {string.Join(", ", daysOfWeeks)}";
        }

        private static int GenerateId()
        {
            return Guid.NewGuid().GetHashCode();
        }
    }
}
