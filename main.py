import asyncio
import aiohttp
import time
from collections import Counter

# Configurations
API_URL = "http://localhost:8000"  # replace with your API
TOTAL_REQUESTS = 1000
CONCURRENT_REQUESTS = 100  # number of concurrent tasks

results = Counter()


async def fetch(session, url, request_id):
    try:
        async with session.get(url) as response:
            results[response.status] += 1
            await response.text()  # Ensure response is read fully
    except Exception as e:
        results["error"] += 1
        print(f"Request {request_id} failed: {e}")


async def bound_fetch(sem, session, url, request_id):
    async with sem:  # limit concurrent access
        await fetch(session, url, request_id)


async def run_benchmark():
    sem = asyncio.Semaphore(CONCURRENT_REQUESTS)
    async with aiohttp.ClientSession() as session:
        tasks = []
        for i in range(TOTAL_REQUESTS):
            task = asyncio.create_task(bound_fetch(sem, session, API_URL, i))
            tasks.append(task)

        start_time = time.perf_counter()
        await asyncio.gather(*tasks)
        duration = time.perf_counter() - start_time

        print("\n--- Benchmark Results ---")
        print(f"Total Requests: {TOTAL_REQUESTS}")
        print(f"Concurrent Requests: {CONCURRENT_REQUESTS}")
        print(f"Duration: {duration:.2f} seconds")
        print("Response Summary:")
        for key, val in results.items():
            print(f"  {key}: {val}")


if __name__ == "__main__":
    asyncio.run(run_benchmark())
