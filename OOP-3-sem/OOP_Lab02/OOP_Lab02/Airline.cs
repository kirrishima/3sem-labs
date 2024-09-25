namespace OOP_Lab02
{
    public partial class Airline
    {
        private string destination;
        private int flightNumber;
        private string planeType;
        private readonly TimeOnly departureTime;
        private readonly DayOfWeek[] daysOfWeeks;
        private const int minInt = int.MinValue;
        private static int objectCount = minInt;
        private readonly int id;

        private static TimeOnly TimeNow => new TimeOnly(DateTime.Now.TimeOfDay.Ticks);

        public string Destination
        {
            get => destination;
            set => destination = value ?? throw new ArgumentException("Destination cannot be null");
        }

        public int FlightNumber
        {
            get => flightNumber;
            set => flightNumber = value > 0 ? value : throw new ArgumentException("Flight number must be greater than 0");
        }

        public string PlaneType
        {
            get => planeType;
            set => planeType = value ?? throw new ArgumentException("Plane type cannot be null");
        }

        public TimeOnly DepartureTime => departureTime;
        public DayOfWeek[] DaysOfWeeks => daysOfWeeks;

        public static int ObjectCount => objectCount;

        public int ID => id;
    }
}
