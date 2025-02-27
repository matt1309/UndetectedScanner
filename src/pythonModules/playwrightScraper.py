import random
import time
from playwright.sync_api import sync_playwright

# Persistent Playwright browser instance
browser = None
page = None

# List of common User-Agent strings (can be expanded for better rotation)
user_agents = [
    "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/58.0.3029.110 Safari/537.3",
    "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/61.0.3163.100 Safari/537.36",
    "Mozilla/5.0 (Windows NT 10.0; Win64; x64; rv:90.0) Gecko/20100101 Firefox/90.0",
    # Add more User-Agents here
]

def random_user_agent():
    """Returns a random User-Agent from the list"""
    return random.choice(user_agents)

def set_random_viewport(page):
    """Set a random viewport size"""
    width = random.randint(1024, 1920)  # Screen width between 1024px and 1920px
    height = random.randint(768, 1080)  # Screen height between 768px and 1080px
    page.set_viewport_size({"width": width, "height": height})

def spoof_browser_fingerprint(page):
    """Spoof browser fingerprint (e.g., screen resolution, timezone)"""
    page.add_init_script("""
        Object.defineProperty(navigator, 'webdriver', { get: () => undefined });
        Object.defineProperty(navigator, 'platform', { get: () => 'Win32' });
        Object.defineProperty(navigator, 'languages', { get: () => ['en-US', 'en'] });
        Object.defineProperty(navigator, 'timezoneOffset', { get: () => -240 });
    """)

def start_driver():
    global browser, page
    if browser is None:
        with sync_playwright() as p:
            browser = p.chromium.launch(headless=True)  # Launch the browser in headless mode
            page = browser.new_page()

            # Set a random user-agent to mimic a real user
            page.set_user_agent(random_user_agent())

            # Set a random viewport size
            set_random_viewport(page)

            # Spoof browser fingerprinting to reduce detection chances
            spoof_browser_fingerprint(page)

            # Enable additional browser features (Optional)
            # page.context.add_init_script('navigator.plugins = [new Proxy({}, { get() { return undefined; } })];')

def playwrightScraper(retry, url, method, headers, useragent, proxy, timeout, request_data):
    """Loads a URL and returns page source with anti-detection features"""
    global browser, page
    try:
        start_driver()  # Ensure driver is running
        page.goto(url, timeout=timeout)  # Visit the URL
        time.sleep(random.uniform(2, 4))  # Simulate human-like wait times
        return {"status": "Ok", "data": page.content()}  # Return the page source
    except Exception as e:
        quit_driver()
        return {"status": "Error", "data": str(e)}

def quit_driver():
    """Quits the Playwright browser instance"""
    global browser, page
    if browser:
        page.close()  # Close the page
        browser.close()  # Close the browser
        browser = None
        page = None
    return {"status": "Ok", "data": "Driver closed"}

