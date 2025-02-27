from undetected_chromedriver.v2 import Chrome
from selenium.webdriver.chrome.service import Service
from webdriver_manager.chrome import ChromeDriverManager
from selenium.webdriver.chrome.options import Options

# Persistent Selenium WebDriver instance
driver = None

def start_driver():
    global driver
    if driver is None:
        options = Options()
        options.add_argument("--headless")  # Run in headless mode (optional)
        options.add_argument("--disable-blink-features=AutomationControlled")
        options.add_argument("--no-sandbox")
        options.add_argument("--disable-dev-shm-usage")
        options.add_argument("--disable-gpu")
        options.add_argument("--remote-debugging-port=9222")
        
        driver = Chrome(service=Service(ChromeDriverManager().install()), options=options)

def seleniumScraper(retry, url, method, headers, useragent, proxy, timeout, request_data):
    """Loads a URL and returns page source"""
    global driver
    try:
        start_driver()  # Ensure driver is running
        driver.get(url)
        return {"status": "Ok", "data": driver.page_source}
    except Exception as e:
        driver.quit()
        driver = None  # Force restart next time
        return {"status": "Error", "data": str(e)}

def quit_driver():
    """Quits the WebDriver"""
    global driver
    if driver:
        driver.quit()
        driver = None
    return {"status": "Ok", "data": "Driver closed"}
