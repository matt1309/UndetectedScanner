from selenium import webdriver
from selenium.webdriver.chrome.service import Service
from webdriver_manager.chrome import ChromeDriverManager
from selenium.webdriver.chrome.options import Options
from selenium_stealth import stealth

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
        
        driver = webdriver.Chrome(service=Service(ChromeDriverManager().install()), options=options)
        
        # Apply stealth settings to avoid detection
        stealth(driver,
                languages=["en-US", "en"],
                vendor="Google Inc.",
                platform="Win32",
                webgl_vendor="Intel Inc.",
                renderer="Intel Iris OpenGL Engine",
                fix_hairline=True)

def seleniumStealth(retry, url, method, headers, useragent, proxy, timeout, request_data):
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
