namespace ServiceState.Common
{
    public interface ITimerObserver
    {
        // true: 終了
        // false: 継続
        bool Timeout(TimerInformation info);
    }
}

