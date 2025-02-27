#ifndef BROWSER_H
#define BROWSER_H

#include <iostream>
#include <string>
#include <map>

class Browser
{
public:
    // need to define all the curl functions i plan on using in here as well as in chrome/firefox.
    virtual bool isReady() = 0;
    // Common functionality/vars here

    virtual std::pair<std::string, std::string> Request(const bool retry, const std::string &url, const std::string &method,
                                                        const std::string &headers,
                                                        const std::string &useragent,
                                                        const std::string &proxy, const int &timeout, const std::string &moderator_channel, const std::string &moderator_channel_telegram, std::string &requestData, int &threadnumber) = 0;



//Browser settings for each supported browser to spoof fingerprints. 


 std::map<std::string, bool> http2{
        {"chrome100",
         true},
        {"chrome101",
         true},

        {"chrome104",
         true},

        {"chrome107",
         true},

        {"chrome110",
         true},
        {"chrome116",
         true},
        {"chrome119",
         true},
        {"chrome120",
         true},
        {"chrome99",
         true},
        {"chrome99a",
         true},

        {"edge101",
         true},
        {"edge99",
         true},

        {"safari15_3",
         true},
        {"safari15_5",
         true},
        {"safari17",
         true},

        {"safari17_2IOS",
         true},
        {"firefox100",
         true},
        {"firefox102",
         true},
        {"firefox109",
         true},
        {"firefox117",
         true},
        {"firefox91esr",
         true},
        {"firefox95",
         true},
        {"firefox98",
         true}

    };



    std::map<std::string, std::string> curves = {
        {"safari15_3", "X25519:P-256:P-384:P-521"},
        {"safari15_5", "X25519:P-256:P-384:P-521"},
        {"safari17", "X25519:P-256:P-384:P-521"},
        {"safari17_2IOS", "X25519:P-256:P-384:P-52"},

    };


   std::map<std::string, std::string> signatureHashes = {
        {"safari15_3", "ecdsa_secp256r1_sha256,rsa_pss_rsae_sha256,rsa_pkcs1_sha256,ecdsa_secp384r1_sha384,ecdsa_sha1,rsa_pss_rsae_sha384,rsa_pss_rsae_sha384,rsa_pkcs1_sha384,rsa_pss_rsae_sha512,rsa_pkcs1_sha512,rsa_pkcs1_sha1"},

        {"safari15_5", "ecdsa_secp256r1_sha256,rsa_pss_rsae_sha256,rsa_pkcs1_sha256,ecdsa_secp384r1_sha384,ecdsa_sha1,rsa_pss_rsae_sha384,rsa_pss_rsae_sha384,rsa_pkcs1_sha384,rsa_pss_rsae_sha512,rsa_pkcs1_sha512,rsa_pkcs1_sha1"},

        {"safari17", "ecdsa_secp256r1_sha256,rsa_pss_rsae_sha256,rsa_pkcs1_sha256,ecdsa_secp384r1_sha384,ecdsa_sha1,rsa_pss_rsae_sha384,rsa_pss_rsae_sha384,rsa_pkcs1_sha384,rsa_pss_rsae_sha512,rsa_pkcs1_sha512,rsa_pkcs1_sha1"},

        {"safari17_2IOS", "ecdsa_secp256r1_sha256,rsa_pss_rsae_sha256,rsa_pkcs1_sha256,ecdsa_secp384r1_sha384,ecdsa_sha1,rsa_pss_rsae_sha384,rsa_pss_rsae_sha384,rsa_pkcs1_sha384,rsa_pss_rsae_sha512,rsa_pkcs1_sha512,rsa_pkcs1_sha1"},

    };


std::map<std::string, bool> npn{
        {"chrome100",
         false},
        {"chrome101",
         false},
        {"chrome104",
         false},
        {"chrome107",
         false},
        {"chrome110",
         false},
        {"chrome116",
         false},

        {"chrome119",
         false},
        {"chrome120",
         false},

        {"chrome99",
         false},
        {"chrome99a",
         false},
        {"edge101",
         false},

        {"edge99",
         false},

    };


      std::map<std::string, bool> alpn{
        {"chrome100",
         true},
        {"chrome101",
         true},
        {"chrome104",
         true},
        {"chrome107",
         true},
        {"chrome110",
         true},
        {"chrome116",
         true},

        {"chrome119",
         true},
        {"chrome120",
         true},

        {"chrome99",
         true},
        {"chrome99a",
         true},
        {"edge101",
         true},

        {"edge99",
         true},

    };

  std::map<std::string, bool> alps{
        {"chrome100",
         true},
        {"chrome101",
         true},
        {"chrome104",
         true},
        {"chrome107",
         true},
        {"chrome110",
         true},
        {"chrome116",
         true},

        {"chrome119",
         true},
        {"chrome120",
         true},

        {"chrome99",
         true},
        {"chrome99a",
         true},
        {"edge101",
         true},

        {"edge99",
         true},

    };

   std::map<std::string, bool> tlsSessionTicket{
        {"chrome110",
         true},
        {"chrome116",
         true},
        {"chrome119",
         true},
        {"chrome120",
         true}};




    std::map<std::string, bool> tlsPermuteExtensions{
        {"chrome110",
         true},
        {"chrome116",
         true},
        {"chrome119",
         true},
        {"chrome120",
         true}};




    std::map<std::string, std::string> certCompression{
        {"chrome100",
         "brotli"},

        {"chrome101",
         "brotli"},

        {"chrome104",
         "brotli"},
        {"chrome107",
         "brotli"},

        {"chrome110",
         "brotli"},
        {"chrome116",
         "brotli"},
        {"chrome119",
         "brotli"},
        {"chrome120",
         "brotli"},

        {"chrome99",
         "brotli"},

        {"chrome99a",
         "brotli"},

        {"edge101",
         "brotli"},

        {"edge99",
         "brotli"},

        {"safari15_5",
         "zlib"},

        {"safari17",
         "zlib"},

        {"safari17_2IOS",
         "zlib"}

    };


 std::map<std::string, std::string> http2Settings{
        {"chrome100",
         "1:65536;3:1000;4:6291456;6:262144"},
        {"chrome101",
         "1:65536;3:1000;4:6291456;6:262144"},

        {"chrome104",
         "1:65536;3:1000;4:6291456;6:262144"},
        {"chrome107",
         "1:65536;2:0;3:1000;4:6291456;6:262144"},

        {"chrome110",
         "1:65536;2:0;3:1000;4:6291456;6:262144"},

        {"chrome116",
         "1:65536;2:0;3:1000;4:6291456;6:262144"},
        {"chrome119",
         "1:65536;2:0;4:6291456;6:262144"},
        {"chrome120",
         "1:65536;2:0;4:6291456;6:262144"},

        {"chrome99",
         "1:65536;3:1000;4:6291456;6:262144"},

        {"chrome99a",
         "1:65536;3:1000;4:6291456;6:262144"},

        {"edge101",
         "1:65536;3:1000;4:6291456;6:262144"},
        {"edge99",
         "1:65536;3:1000;4:6291456;6:262144"},

        {"safari15_3",
         "4:4194304;3:100"},
        {"safari15_5",
         "4:4194304;3:100"},

        {"safari17",
         "2:0;4:4194304;3:100"},

        {"safari17_2IOS",
         "2:0;4:2097152;3:100"},

    };














    std::map<std::string, std::string> ciphers{
    {
        "chrome100",
        "TLS_AES_128_GCM_SHA256:TLS_AES_256_GCM_SHA384:TLS_CHACHA20_POLY1305_SHA256:ECDHE-ECDSA-AES128-GCM-SHA256:ECDHE-RSA-AES128-GCM-SHA256:ECDHE-ECDSA-AES256-GCM-SHA384:ECDHE-RSA-AES256-GCM-SHA384:ECDHE-ECDSA-CHACHA20-POLY1305:ECDHE-RSA-CHACHA20-POLY1305:ECDHE-RSA-AES128-SHA:ECDHE-RSA-AES256-SHA:AES128-GCM-SHA256:AES256-GCM-SHA384:AES128-SHA:AES256-SHA",
    },
    {"chrome101",
     "TLS_AES_128_GCM_SHA256:TLS_AES_256_GCM_SHA384:TLS_CHACHA20_POLY1305_SHA256:ECDHE-ECDSA-AES128-GCM-SHA256:ECDHE-RSA-AES128-GCM-SHA256:ECDHE-ECDSA-AES256-GCM-SHA384:ECDHE-RSA-AES256-GCM-SHA384:ECDHE-ECDSA-CHACHA20-POLY1305:ECDHE-RSA-CHACHA20-POLY1305:ECDHE-RSA-AES128-SHA:ECDHE-RSA-AES256-SHA:AES128-GCM-SHA256:AES256-GCM-SHA384:AES128-SHA:AES256-SHA"},

    {"chrome104",
     "TLS_AES_128_GCM_SHA256:TLS_AES_256_GCM_SHA384:TLS_CHACHA20_POLY1305_SHA256:ECDHE-ECDSA-AES128-GCM-SHA256:ECDHE-RSA-AES128-GCM-SHA256:ECDHE-ECDSA-AES256-GCM-SHA384:ECDHE-RSA-AES256-GCM-SHA384:ECDHE-ECDSA-CHACHA20-POLY1305:ECDHE-RSA-CHACHA20-POLY1305:ECDHE-RSA-AES128-SHA:ECDHE-RSA-AES256-SHA:AES128-GCM-SHA256:AES256-GCM-SHA384:AES128-SHA:AES256-SHA"},

    {"chrome107",
     "TLS_AES_128_GCM_SHA256:TLS_AES_256_GCM_SHA384:TLS_CHACHA20_POLY1305_SHA256:ECDHE-ECDSA-AES128-GCM-SHA256:ECDHE-RSA-AES128-GCM-SHA256:ECDHE-ECDSA-AES256-GCM-SHA384:ECDHE-RSA-AES256-GCM-SHA384:ECDHE-ECDSA-CHACHA20-POLY1305:ECDHE-RSA-CHACHA20-POLY1305:ECDHE-RSA-AES128-SHA:ECDHE-RSA-AES256-SHA:AES128-GCM-SHA256:AES256-GCM-SHA384:AES128-SHA:AES256-SHA"},

    {"chrome110",
     "TLS_AES_128_GCM_SHA256:TLS_AES_256_GCM_SHA384:TLS_CHACHA20_POLY1305_SHA256:ECDHE-ECDSA-AES128-GCM-SHA256:ECDHE-RSA-AES128-GCM-SHA256:ECDHE-ECDSA-AES256-GCM-SHA384:ECDHE-RSA-AES256-GCM-SHA384:ECDHE-ECDSA-CHACHA20-POLY1305:ECDHE-RSA-CHACHA20-POLY1305:ECDHE-RSA-AES128-SHA:ECDHE-RSA-AES256-SHA:AES128-GCM-SHA256:AES256-GCM-SHA384:AES128-SHA:AES256-SHA"},

    {"chrome116",
     "TLS_AES_128_GCM_SHA256:TLS_AES_256_GCM_SHA384:TLS_CHACHA20_POLY1305_SHA256:ECDHE-ECDSA-AES128-GCM-SHA256:ECDHE-RSA-AES128-GCM-SHA256:ECDHE-ECDSA-AES256-GCM-SHA384:ECDHE-RSA-AES256-GCM-SHA384:ECDHE-ECDSA-CHACHA20-POLY1305:ECDHE-RSA-CHACHA20-POLY1305:ECDHE-RSA-AES128-SHA:ECDHE-RSA-AES256-SHA:AES128-GCM-SHA256:AES256-GCM-SHA384:AES128-SHA:AES256-SHA"},

    {"chrome119",
     "TLS_AES_128_GCM_SHA256:TLS_AES_256_GCM_SHA384:TLS_CHACHA20_POLY1305_SHA256:ECDHE-ECDSA-AES128-GCM-SHA256:ECDHE-RSA-AES128-GCM-SHA256:ECDHE-ECDSA-AES256-GCM-SHA384:ECDHE-RSA-AES256-GCM-SHA384:ECDHE-ECDSA-CHACHA20-POLY1305:ECDHE-RSA-CHACHA20-POLY1305:ECDHE-RSA-AES128-SHA:ECDHE-RSA-AES256-SHA:AES128-GCM-SHA256:AES256-GCM-SHA384:AES128-SHA:AES256-SHA"},
    {"chrome120",
     "TLS_AES_128_GCM_SHA256:TLS_AES_256_GCM_SHA384:TLS_CHACHA20_POLY1305_SHA256:ECDHE-ECDSA-AES128-GCM-SHA256:ECDHE-RSA-AES128-GCM-SHA256:ECDHE-ECDSA-AES256-GCM-SHA384:ECDHE-RSA-AES256-GCM-SHA384:ECDHE-ECDSA-CHACHA20-POLY1305:ECDHE-RSA-CHACHA20-POLY1305:ECDHE-RSA-AES128-SHA:ECDHE-RSA-AES256-SHA:AES128-GCM-SHA256:AES256-GCM-SHA384:AES128-SHA:AES256-SHA"},

    {"chrome99",
     "TLS_AES_128_GCM_SHA256:TLS_AES_256_GCM_SHA384:TLS_CHACHA20_POLY1305_SHA256:ECDHE-ECDSA-AES128-GCM-SHA256:ECDHE-RSA-AES128-GCM-SHA256:ECDHE-ECDSA-AES256-GCM-SHA384:ECDHE-RSA-AES256-GCM-SHA384:ECDHE-ECDSA-CHACHA20-POLY1305:ECDHE-RSA-CHACHA20-POLY1305:ECDHE-RSA-AES128-SHA:ECDHE-RSA-AES256-SHA:AES128-GCM-SHA256:AES256-GCM-SHA384:AES128-SHA:AES256-SHA"},

    {"chrome99a",
     "TLS_AES_128_GCM_SHA256:TLS_AES_256_GCM_SHA384:TLS_CHACHA20_POLY1305_SHA256:ECDHE-ECDSA-AES128-GCM-SHA256:ECDHE-RSA-AES128-GCM-SHA256:ECDHE-ECDSA-AES256-GCM-SHA384:ECDHE-RSA-AES256-GCM-SHA384:ECDHE-ECDSA-CHACHA20-POLY1305:ECDHE-RSA-CHACHA20-POLY1305:ECDHE-RSA-AES128-SHA:ECDHE-RSA-AES256-SHA:AES128-GCM-SHA256:AES256-GCM-SHA384:AES128-SHA:AES256-SHA"},

    {"edge101",
     "TLS_AES_128_GCM_SHA256:TLS_AES_256_GCM_SHA384:TLS_CHACHA20_POLY1305_SHA256:ECDHE-ECDSA-AES128-GCM-SHA256:ECDHE-RSA-AES128-GCM-SHA256:ECDHE-ECDSA-AES256-GCM-SHA384:ECDHE-RSA-AES256-GCM-SHA384:ECDHE-ECDSA-CHACHA20-POLY1305:ECDHE-RSA-CHACHA20-POLY1305:ECDHE-RSA-AES128-SHA:ECDHE-RSA-AES256-SHA:AES128-GCM-SHA256:AES256-GCM-SHA384:AES128-SHA:AES256-SHA"},

    {"edge99",
     "TLS_AES_128_GCM_SHA256:TLS_AES_256_GCM_SHA384:TLS_CHACHA20_POLY1305_SHA256:ECDHE-ECDSA-AES128-GCM-SHA256:ECDHE-RSA-AES128-GCM-SHA256:ECDHE-ECDSA-AES256-GCM-SHA384:ECDHE-RSA-AES256-GCM-SHA384:ECDHE-ECDSA-CHACHA20-POLY1305:ECDHE-RSA-CHACHA20-POLY1305:ECDHE-RSA-AES128-SHA:ECDHE-RSA-AES256-SHA:AES128-GCM-SHA256:AES256-GCM-SHA384:AES128-SHA:AES256-SHA"},

    {"safari15_3",
     "TLS_AES_128_GCM_SHA256:TLS_AES_256_GCM_SHA384:TLS_CHACHA20_POLY1305_SHA256:TLS_ECDHE_ECDSA_WITH_AES_256_GCM_SHA384:TLS_ECDHE_ECDSA_WITH_AES_128_GCM_SHA256:TLS_ECDHE_ECDSA_WITH_CHACHA20_POLY1305_SHA256:TLS_ECDHE_RSA_WITH_AES_256_GCM_SHA384:TLS_ECDHE_RSA_WITH_AES_128_GCM_SHA256:TLS_ECDHE_RSA_WITH_CHACHA20_POLY1305_SHA256:TLS_ECDHE_ECDSA_WITH_AES_256_CBC_SHA384:TLS_ECDHE_ECDSA_WITH_AES_128_CBC_SHA256:TLS_ECDHE_ECDSA_WITH_AES_256_CBC_SHA:TLS_ECDHE_ECDSA_WITH_AES_128_CBC_SHA:TLS_ECDHE_RSA_WITH_AES_256_CBC_SHA384:TLS_ECDHE_RSA_WITH_AES_128_CBC_SHA256:TLS_ECDHE_RSA_WITH_AES_256_CBC_SHA:TLS_ECDHE_RSA_WITH_AES_128_CBC_SHA:TLS_RSA_WITH_AES_256_GCM_SHA384:TLS_RSA_WITH_AES_128_GCM_SHA256:TLS_RSA_WITH_AES_256_CBC_SHA256:TLS_RSA_WITH_AES_128_CBC_SHA256:TLS_RSA_WITH_AES_256_CBC_SHA:TLS_RSA_WITH_AES_128_CBC_SHA:TLS_ECDHE_ECDSA_WITH_3DES_EDE_CBC_SHA:TLS_ECDHE_RSA_WITH_3DES_EDE_CBC_SHA:TLS_RSA_WITH_3DES_EDE_CBC_SHA"},

    {"safari15_5",
     "TLS_AES_128_GCM_SHA256:TLS_AES_256_GCM_SHA384:TLS_CHACHA20_POLY1305_SHA256:TLS_ECDHE_ECDSA_WITH_AES_256_GCM_SHA384:TLS_ECDHE_ECDSA_WITH_AES_128_GCM_SHA256:TLS_ECDHE_ECDSA_WITH_CHACHA20_POLY1305_SHA256:TLS_ECDHE_RSA_WITH_AES_256_GCM_SHA384:TLS_ECDHE_RSA_WITH_AES_128_GCM_SHA256:TLS_ECDHE_RSA_WITH_CHACHA20_POLY1305_SHA256:TLS_ECDHE_ECDSA_WITH_AES_256_CBC_SHA:TLS_ECDHE_ECDSA_WITH_AES_128_CBC_SHA:TLS_ECDHE_RSA_WITH_AES_256_CBC_SHA:TLS_ECDHE_RSA_WITH_AES_128_CBC_SHA:TLS_RSA_WITH_AES_256_GCM_SHA384:TLS_RSA_WITH_AES_128_GCM_SHA256:TLS_RSA_WITH_AES_256_CBC_SHA:TLS_RSA_WITH_AES_128_CBC_SHA:TLS_ECDHE_ECDSA_WITH_3DES_EDE_CBC_SHA:TLS_ECDHE_RSA_WITH_3DES_EDE_CBC_SHA:TLS_RSA_WITH_3DES_EDE_CBC_SHA"},

    {"safari17",
     "TLS_AES_128_GCM_SHA256:TLS_AES_256_GCM_SHA384:TLS_CHACHA20_POLY1305_SHA256:TLS_ECDHE_ECDSA_WITH_AES_256_GCM_SHA384:TLS_ECDHE_ECDSA_WITH_AES_128_GCM_SHA256:TLS_ECDHE_ECDSA_WITH_CHACHA20_POLY1305_SHA256:TLS_ECDHE_RSA_WITH_AES_256_GCM_SHA384:TLS_ECDHE_RSA_WITH_AES_128_GCM_SHA256:TLS_ECDHE_RSA_WITH_CHACHA20_POLY1305_SHA256:TLS_ECDHE_ECDSA_WITH_AES_256_CBC_SHA:TLS_ECDHE_ECDSA_WITH_AES_128_CBC_SHA:TLS_ECDHE_RSA_WITH_AES_256_CBC_SHA:TLS_ECDHE_RSA_WITH_AES_128_CBC_SHA:TLS_RSA_WITH_AES_256_GCM_SHA384:TLS_RSA_WITH_AES_128_GCM_SHA256:TLS_RSA_WITH_AES_256_CBC_SHA:TLS_RSA_WITH_AES_128_CBC_SHA:TLS_ECDHE_ECDSA_WITH_3DES_EDE_CBC_SHA:TLS_ECDHE_RSA_WITH_3DES_EDE_CBC_SHA:TLS_RSA_WITH_3DES_EDE_CBC_SHA"},

    {"safari17_2IOS",
     "TLS_AES_128_GCM_SHA256:TLS_AES_256_GCM_SHA384:TLS_CHACHA20_POLY1305_SHA256:TLS_ECDHE_ECDSA_WITH_AES_256_GCM_SHA384:TLS_ECDHE_ECDSA_WITH_AES_128_GCM_SHA256:TLS_ECDHE_ECDSA_WITH_CHACHA20_POLY1305_SHA256:TLS_ECDHE_RSA_WITH_AES_256_GCM_SHA384:TLS_ECDHE_RSA_WITH_AES_128_GCM_SHA256:TLS_ECDHE_RSA_WITH_CHACHA20_POLY1305_SHA256:TLS_ECDHE_ECDSA_WITH_AES_256_CBC_SHA:TLS_ECDHE_ECDSA_WITH_AES_128_CBC_SHA:TLS_ECDHE_RSA_WITH_AES_256_CBC_SHA:TLS_ECDHE_RSA_WITH_AES_128_CBC_SHA:TLS_RSA_WITH_AES_256_GCM_SHA384:TLS_RSA_WITH_AES_128_GCM_SHA256:TLS_RSA_WITH_AES_256_CBC_SHA:TLS_RSA_WITH_AES_128_CBC_SHA:TLS_ECDHE_ECDSA_WITH_3DES_EDE_CBC_SHA:TLS_ECDHE_RSA_WITH_3DES_EDE_CBC_SHA:TLS_RSA_WITH_3DES_EDE_CBC_SHA"},

    {"firefox100",
     "aes_128_gcm_sha_256:chacha20_poly1305_sha_256:aes_256_gcm_sha_384:ecdhe_ecdsa_aes_128_gcm_sha_256:ecdhe_rsa_aes_128_gcm_sha_256:ecdhe_ecdsa_chacha20_poly1305_sha_256:ecdhe_rsa_chacha20_poly1305_sha_256:ecdhe_ecdsa_aes_256_gcm_sha_384:ecdhe_rsa_aes_256_gcm_sha_384:ecdhe_ecdsa_aes_256_sha:ecdhe_ecdsa_aes_128_sha:ecdhe_rsa_aes_128_sha:ecdhe_rsa_aes_256_sha:rsa_aes_128_gcm_sha_256:rsa_aes_256_gcm_sha_384:rsa_aes_128_sha:rsa_aes_256_sha"},

    {"firefox102",
     "aes_128_gcm_sha_256:chacha20_poly1305_sha_256:aes_256_gcm_sha_384:ecdhe_ecdsa_aes_128_gcm_sha_256:ecdhe_rsa_aes_128_gcm_sha_256:ecdhe_ecdsa_chacha20_poly1305_sha_256:ecdhe_rsa_chacha20_poly1305_sha_256:ecdhe_ecdsa_aes_256_gcm_sha_384:ecdhe_rsa_aes_256_gcm_sha_384:ecdhe_ecdsa_aes_256_sha:ecdhe_ecdsa_aes_128_sha:ecdhe_rsa_aes_128_sha:ecdhe_rsa_aes_256_sha:rsa_aes_128_gcm_sha_256:rsa_aes_256_gcm_sha_384:rsa_aes_128_sha:rsa_aes_256_sha"},

    {"firefox109",
     "aes_128_gcm_sha_256:chacha20_poly1305_sha_256:aes_256_gcm_sha_384:ecdhe_ecdsa_aes_128_gcm_sha_256:ecdhe_rsa_aes_128_gcm_sha_256:ecdhe_ecdsa_chacha20_poly1305_sha_256:ecdhe_rsa_chacha20_poly1305_sha_256:ecdhe_ecdsa_aes_256_gcm_sha_384:ecdhe_rsa_aes_256_gcm_sha_384:ecdhe_ecdsa_aes_256_sha:ecdhe_ecdsa_aes_128_sha:ecdhe_rsa_aes_128_sha:ecdhe_rsa_aes_256_sha:rsa_aes_128_gcm_sha_256:rsa_aes_256_gcm_sha_384:rsa_aes_128_sha:rsa_aes_256_sha"}
     };

   

   

    std::map<std::string, long> http2WindowsUpdate{
    {"chrome100", 15663105},
    {"chrome101", 15663105},
    {"chrome104", 15663105},
    {"chrome107", 15663105},
    {"chrome110", 15663105},
    {"chrome116", 15663105},
    {"chrome119", 15663105},
    {"chrome120", 15663105},
    {"chrome99", 15663105},
    {"chrome99a", 15663105},
    {"edge101", 15663105},
    {"edge99", 15663105},
    {"safari15_3", 10485760},
    {"safari15_5", 10485760},
    {"safari17", 10485760},
    {"safari17_2IOS", 10485760}
};

    std::map<std::string, bool> compressed{
        {"chrome100",
         true},

        {"chrome101",
         true},

        {"chrome104",
         true},

        {"chrome107",
         true},

        {"chrome110",
         true},

        {"chrome116",
         true},

        {"chrome119",
         true},

        {"chrome120",
         true},

        {"chrome99",
         true},

        {"chrome99a",
         true},

        {"edge101",
         true},

        {"edge99",
         true},

        {"safari15_3",
         true},

        {"safari15_5",
         true},

        {"safari17",
         true},

        {"safari17_2IOS",
         true},

        {"firefox100",
         true},
        {"firefox102",
         true},
        {"firefox109",
         true},
        {"firefox117",
         true},

        {"firefox91esr",
         true},

        {"firefox95",
         true},
        {"firefox98",
         true}

    };

    std::map<std::string, std::string> ech{
        {"chrome119", "GREASE"},
        {"chrome120", "GREASE"}

    };

    std::map<std::string, bool> tlsGrease{
        {"chrome110", true}

    };

    std::map<std::string, std::string> tlsVersion{
        {"chrome100", "1.2"},
        {"chrome101", "1.2"},
        {"chrome104", "1.2"},
        {"chrome107", "1.2"},
        {"chrome110", "1.2"},
        {"chrome116", "1.2"},
        {"chrome119", "1.2"},
        {"chrome120", "1.2"},
        {"chrome99", "1.2"},
        {"chrome99a", "1.2"},
        {"edge101", "1.2"},
        {"edge99", "1.2"},
        {"safari15_3", "1.0"},
        {"safari15_5", "1.0"},
        {"safari17", "1.0"},
        {"safari17_2IOS", "1.0"},

    };

  





     



         
 


    std::map<std::string, std::string> http2PseudoHeaderOrder = {
        {"safari15_3", "mspa"},
        {"safari15_5", "mspa"},
        {"safari17", "mspa"},
        {"safari17_2IOS", "mspa"}

    };


 

    std::map<std::string, bool> noTLSSessionTicket = {
        {"safari15_3", true},
        {"safari15_5", true},
        {"safari17", true},
        {"safari17_2IOS", true},
    };

 
    std::map<std::string, std::string> headers = {
        // chrome 100
        {"chrome100",
         "sec-ch-ua: \" Not A;Brand\";v=\"99\", \"Chromium\";v=\"100\", \"Google Chrome\";v=\"100\"\n"
         "sec-ch-ua-mobile: ?0\n"
         "sec-ch-ua-platform: \"Windows\"\n"
         "Upgrade-Insecure-Requests: 1\n"
         "User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/100.0.4896.75 Safari/537.36\n"
         "Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/avif,image/webp,image/apng,*/*;q=0.8,application/signed-exchange;v=b3;q=0.9\n"
         "Sec-Fetch-Site: none\n"
         "Sec-Fetch-Mode: navigate\n"
         "Sec-Fetch-User: ?1\n"
         "Sec-Fetch-Dest: document\n"
         "Accept-Encoding: gzip, deflate, br\n"
         "Accept-Language: en-US,en;q=0.9\n"},

        // chrome 101
        {"chrome101",
         "sec-ch-ua: \" Not A;Brand\";v=\"99\", \"Chromium\";v=\"101\", \"Google Chrome\";v=\"101\"\n"
         "sec-ch-ua-mobile: ?0\n"
         "sec-ch-ua-platform: \"Windows\"\n"
         "Upgrade-Insecure-Requests: 1\n"
         "User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/101.0.4951.67 Safari/537.36\n"
         "Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/avif,image/webp,image/apng,*/*;q=0.8,application/signed-exchange;v=b3;q=0.9\n"
         "Sec-Fetch-Site: none\n"
         "Sec-Fetch-Mode: navigate\n"
         "Sec-Fetch-User: ?1\n"
         "Sec-Fetch-Dest: document\n"
         "Accept-Encoding: gzip, deflate, br\n"
         "Accept-Language: en-US,en;q=0.9\n"},

        // chrome 104
        {"chrome104",
         "sec-ch-ua: \"Chromium\";v=\"104\", \" Not A;Brand\";v=\"99\", \"Google Chrome\";v=\"104\"\n"
         "sec-ch-ua-mobile: ?0\n"
         "sec-ch-ua-platform: \"Windows\"\n"
         "Upgrade-Insecure-Requests: 1\n"
         "User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/104.0.0.0 Safari/537.36\n"
         "Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/avif,image/webp,image/apng,*/*;q=0.8,application/signed-exchange;v=b3;q=0.9\n"
         "Sec-Fetch-Site: none\n"
         "Sec-Fetch-Mode: navigate\n"
         "Sec-Fetch-User: ?1\n"
         "Sec-Fetch-Dest: document\n"
         "Accept-Encoding: gzip, deflate, br\n"
         "Accept-Language: en-US,en;q=0.9\n"},

        // chrome 107
        {"chrome107",
         "sec-ch-ua: \"Google Chrome\";v=\"107\", \"Chromium\";v=\"107\", \"Not=A?Brand\";v=\"24\"\n"
         "sec-ch-ua-mobile: ?0\n"
         "sec-ch-ua-platform: \"Windows\"\n"
         "Upgrade-Insecure-Requests: 1\n"
         "User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/107.0.0.0 Safari/537.36\n"
         "Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/avif,image/webp,image/apng,*/*;q=0.8,application/signed-exchange;v=b3;q=0.9\n"
         "Sec-Fetch-Site: none\n"
         "Sec-Fetch-Mode: navigate\n"
         "Sec-Fetch-User: ?1\n"
         "Sec-Fetch-Dest: document\n"
         "Accept-Encoding: gzip, deflate, br\n"
         "Accept-Language: en-US,en;q=0.9\n"},

        // chrome 110
        {"chrome110",
         "sec-ch-ua: \"Chromium\";v=\"110\", \"Not A(Brand\";v=\"24\", \"Google Chrome\";v=\"110\"\n"
         "sec-ch-ua-mobile: ?0\n"
         "sec-ch-ua-platform: \"Windows\"\n"
         "Upgrade-Insecure-Requests: 1\n"
         "User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/110.0.0.0 Safari/537.36\n"
        "Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/avif,image/webp,image/apng,*/*;q=0.8,application/signed-exchange;v=b3;q=0.7\n"
         "Sec-Fetch-Site: none\n"
         "Sec-Fetch-Mode: navigate\n"
         "Sec-Fetch-User: ?1\n"
         "Sec-Fetch-Dest: document\n"
         "Accept-Encoding: gzip, deflate, br\n"
         "Accept-Language: en-US,en;q=0.9\n"},



/* {"chrome110",
"Content-Type: application/json\n"
"authority: www.masterofmalt.com\n"
*/
// "accept: */*\n"
/*
"accept-language: en-US,en;q=0.9\n"
"content-type: application/json\n"
"origin: https://www.masterofmalt.com\n"
"referer: https://www.masterofmalt.com/s/?q=springbank&size=n_25_n\n"
"sec-ch-ua: \"Chromium\";v=\"110\", \"Not A(Brand\";v=\"24\", \"Google Chrome\";v=\"110\"\n"
"sec-ch-ua-mobile: ?0\n"
"sec-ch-ua-platform: \"Windows\"\n"
"sec-fetch-dest: empty\n"
"sec-fetch-mode: cors\n"
"sec-fetch-site: same-origin\n"
"user-agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/100.0.4896.75 Safari/537.36\n"
"x-swiftype-client: elastic-app-search-javascript\n"
"x-swiftype-client-version: 7.17.0\n"

},
*/

        // chrome 116
        {"chrome116",
         "sec-ch-ua: \"Chromium\";v=\"116\", \"Not)A;Brand\";v=\"24\", \"Google Chrome\";v=\"116\"\n"
         "sec-ch-ua-mobile: ?0\n"
         "sec-ch-ua-platform: \"Windows\"\n"
         "Upgrade-Insecure-Requests: 1\n"
         "User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/116.0.0.0 Safari/537.36\n"
         "Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/avif,image/webp,image/apng,*/*;q=0.8,application/signed-exchange;v=b3;q=0.7\n"
         "Sec-Fetch-Site: none\n"
         "Sec-Fetch-Mode: navigate\n"
         "Sec-Fetch-User: ?1\n"
         "Sec-Fetch-Dest: document\n"
         "Accept-Encoding: gzip, deflate, br\n"
         "Accept-Language: en-US,en;q=0.9\n"},

        // chrome 119
        {"chrome119",
         "sec-ch-ua: \"Google Chrome\";v=\"119\", \"Chromium\";v=\"119\", \"Not?A_Brand\";v=\"24\"\n"
         "sec-ch-ua-mobile: ?0\n"
         "sec-ch-ua-platform: \"macOS\"\n"
         "Upgrade-Insecure-Requests: 1\n"
         "User-Agent: Mozilla/5.0 (Macintosh; Intel Mac OS X 10_15_7) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/119.0.0.0 Safari/537.36\n"
         "Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/avif,image/webp,image/apng,*/*;q=0.8,application/signed-exchange;v=b3;q=0.7\n"
         "Sec-Fetch-Site: none\n"
         "Sec-Fetch-Mode: navigate\n"
         "Sec-Fetch-User: ?1\n"
         "Sec-Fetch-Dest: document\n"
         "Accept-Encoding: gzip, deflate, br\n"
         "Accept-Language: en-US,en;q=0.9\n"},

        // chrome 120
        {"chrome120",
         "sec-ch-ua: \"Not_A Brand\";v=\"8\", \"Chromium\";v=\"120\", \"Google Chrome\";v=\"120\"\n"
         "sec-ch-ua-mobile: ?0\n"
         "sec-ch-ua-platform: \"macOS\"\n"
         "Upgrade-Insecure-Requests: 1\n"
         "User-Agent: Mozilla/5.0 (Macintosh; Intel Mac OS X 10_15_7) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/120.0.0.0 Safari/537.36\n"
         "Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/avif,image/webp,image/apng,*/*;q=0.8,application/signed-exchange;v=b3;q=0.7\n"
         "Sec-Fetch-Site: none\n"
         "Sec-Fetch-Mode: navigate\n"
         "Sec-Fetch-User: ?1\n"
         "Sec-Fetch-Dest: document\n"
         "Accept-Encoding: gzip, deflate, br\n"
         "Accept-Language: en-US,en;q=0.9\n"},

        // chrome 99
        {"chrome99",
         "sec-ch-ua: \" Not A;Brand\";v=\"99\", \"Chromium\";v=\"99\", \"Google Chrome\";v=\"99\"\n"
         "sec-ch-ua-mobile: ?0\n"
         "sec-ch-ua-platform: \"Windows\"\n"
         "Upgrade-Insecure-Requests: 1\n"
         "User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/99.0.4844.51 Safari/537.36\n"
         "Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/avif,image/webp,image/apng,*/*;q=0.8,application/signed-exchange;v=b3;q=0.9\n"
         "Sec-Fetch-Site: none\n"
         "Sec-Fetch-Mode: navigate\n"
         "Sec-Fetch-User: ?1\n"
         "Sec-Fetch-Dest: document\n"
         "Accept-Encoding: gzip, deflate, br\n"
         "Accept-Language: en-US,en;q=0.9\n"},

        // chrome 99Android
        {"chrome99android",
         "sec-ch-ua: \" Not A;Brand\";v=\"99\", \"Chromium\";v=\"99\", \"Google Chrome\";v=\"99\"\n"
         "sec-ch-ua-mobile: ?1\n"
         "sec-ch-ua-platform: \"Android\"\n"
         "Upgrade-Insecure-Requests: 1\n"
         "User-Agent: Mozilla/5.0 (Linux; Android 12; Pixel 6) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/99.0.4844.58 Mobile Safari/537.36\n"
         "Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/avif,image/webp,image/apng,*/*;q=0.8,application/signed-exchange;v=b3;q=0.9\n"
         "Sec-Fetch-Site: none\n"
         "Sec-Fetch-Mode: navigate\n"
         "Sec-Fetch-User: ?1\n"
         "Sec-Fetch-Dest: document\n"
         "Accept-Encoding: gzip, deflate, br\n"
         "Accept-Language: en-US,en;q=0.9\n"},

        // Edge101
        {"edge101",
         "sec-ch-ua: \" Not A;Brand\";v=\"99\", \"Chromium\";v=\"101\", \"Microsoft Edge\";v=\"101\"\n"
         "sec-ch-ua-mobile: ?0\n"
         "sec-ch-ua-platform: \"Windows\"\n"
         "Upgrade-Insecure-Requests: 1\n"
         "User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/101.0.4951.64 Safari/537.36 Edg/101.0.1210.47\n"
         "Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,image/apng,*/*;q=0.8,application/signed-exchange;v=b3;q=0.9\n"
         "Sec-Fetch-Site: none\n"
         "Sec-Fetch-Mode: navigate\n"
         "Sec-Fetch-User: ?1\n"
         "Sec-Fetch-Dest: document\n"
         "Accept-Encoding: gzip, deflate, br\n"
         "Accept-Language: en-US,en;q=0.9\n"},

        // Edge99
        {"edge99",
         "sec-ch-ua: \" Not A;Brand\";v=\"99\", \"Chromium\";v=\"99\", \"Microsoft Edge\";v=\"99\"\n"
         "sec-ch-ua-mobile: ?0\n"
         "sec-ch-ua-platform: \"Windows\"\n"
         "Upgrade-Insecure-Requests: 1\n"
         "User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/99.0.4844.51 Safari/537.36 Edg/99.0.1150.30\n"
         "Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,image/apng,*/*;q=0.8,application/signed-exchange;v=b3;q=0.9\n"
         "Sec-Fetch-Site: none\n"
         "Sec-Fetch-Mode: navigate\n"
         "Sec-Fetch-User: ?1\n"
         "Sec-Fetch-Dest: document\n"
         "Accept-Encoding: gzip, deflate, br\n"
         "Accept-Language: en-US,en;q=0.9\n"},

        // Safari15_3
        {"safari15_3",
         "User-Agent: Mozilla/5.0 (Macintosh; Intel Mac OS X 10_15_7) AppleWebKit/605.1.15 (KHTML, like Gecko) Version/15.3 Safari/605.1.15\n"
         "Accept: text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8\n"
         "Accept-Language: en-us\n"
         "Accept-Encoding: gzip, deflate, br\n"},

        // Safari15_5
        {"safari15_5",
         "User-Agent: Mozilla/5.0 (Macintosh; Intel Mac OS X 10_15_7) AppleWebKit/605.1.15 (KHTML, like Gecko) Version/15.5 Safari/605.1.15\n"
         "Accept: text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8\n"
         "Accept-Language: en-GB,en-US;q=0.9,en;q=0.8\n"
         "Accept-Encoding: gzip, deflate, br\n"},

        // Safari17
        {"safari17",
         "User-Agent: Mozilla/5.0 (Macintosh; Intel Mac OS X 10_15_7) AppleWebKit/605.1.15 (KHTML, like Gecko) Version/15.5 Safari/605.1.15\n"
         "Accept: text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8\n"
         "Accept-Language: en-GB,en-US;q=0.9,en;q=0.8\n"
         "Accept-Encoding: gzip, deflate, br\n"},

        // Safari17_2IOS
        {"safari17_2IOS",
         "Accept: text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8\n"
         "Sec-Fetch-Site: none\n"
         "Accept-Encoding: gzip, deflate, br\n"
         "Sec-Fetch-Mode: navigate\n"
         "User-Agent: Mozilla/5.0 (iPhone; CPU iPhone OS 17_2 like Mac OS X) AppleWebKit/605.1.15 (KHTML, like Gecko) Version/17.2 Mobile/15E148 Safari/604.1\n"
         "Accept-Language: en-US,en;q=0.9\n"
         "Sec-Fetch-Dest: document\n"},

        // Firefox100
        {"firefox100",
         "User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64; rv:100.0) Gecko/20100101 Firefox/100.0\n"
         "Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/avif,image/webp,*/*;q=0.8\n"
         "Accept-Language: en-US,en;q=0.5\n"
         "Accept-Encoding: gzip, deflate, br\n"
         "Upgrade-Insecure-Requests: 1\n"
         "Sec-Fetch-Dest: document\n"
         "Sec-Fetch-Mode: navigate\n"
         "Sec-Fetch-Site: none\n"
         "Sec-Fetch-User: ?1\n"
         "TE: Trailers\n"},

        // Firefox102
        {"firefox102",
         "User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64; rv:102.0) Gecko/20100101 Firefox/102.0\n"
         "Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/avif,image/webp,*/*;q=0.8\n"
         "Accept-Language: en-US,en;q=0.5\n"
         "Accept-Encoding: gzip, deflate, br\n"
         "Upgrade-Insecure-Requests: 1\n"
         "Sec-Fetch-Dest: document\n"
         "Sec-Fetch-Mode: navigate\n"
         "Sec-Fetch-Site: none\n"
         "Sec-Fetch-User: ?1\n"
         "TE: Trailers\n"},

        // Firefox109
        {"firefox109",
         "User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64; rv:109.0) Gecko/20100101 Firefox/109.0\n"
         "Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/avif,image/webp,*/*;q=0.8\n"
         "Accept-Language: en-US,en;q=0.5\n"
         "Accept-Encoding: gzip, deflate, br\n"
         "Upgrade-Insecure-Requests: 1\n"
         "Sec-Fetch-Dest: document\n"
         "Sec-Fetch-Mode: navigate\n"
         "Sec-Fetch-Site: none\n"
         "Sec-Fetch-User: ?1\n"
         "TE: Trailers\n"},

        // Firefox117
        {"firefox117",
         "User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64; rv:109.0) Gecko/20100101 Firefox/117.0\n"
         "Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/avif,image/webp,*/*;q=0.8\n"
         "Accept-Language: en-US,en;q=0.5\n"
         "Accept-Encoding: gzip, deflate, br\n"
         "Upgrade-Insecure-Requests: 1\n"
         "Sec-Fetch-Dest: document\n"
         "Sec-Fetch-Mode: navigate\n"
         "Sec-Fetch-Site: none\n"
         "Sec-Fetch-User: ?1\n"
         "TE: Trailers\n"},

        // Firefox91esr
        {"firefox91esr",
         "User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64; rv:91.0) Gecko/20100101 Firefox/91.0\n"
         "Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,*/*;q=0.8\n"
         "Accept-Language: en-US,en;q=0.5\n"
         "Accept-Encoding: gzip, deflate, br\n"
         "Upgrade-Insecure-Requests: 1\n"
         "Sec-Fetch-Dest: document\n"
         "Sec-Fetch-Mode: navigate\n"
         "Sec-Fetch-Site: none\n"
         "Sec-Fetch-User: ?1\n"
         "TE: Trailers\n"},

        // Firefox95
        {"firefox95",
         "User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64; rv:95.0) Gecko/20100101 Firefox/95.0\n"
         "Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/avif,image/webp,*/*;q=0.8\n"
         "Accept-Language: en-US,en;q=0.5\n"
         "Accept-Encoding: gzip, deflate, br\n"
         "Upgrade-Insecure-Requests: 1\n"
         "Sec-Fetch-Dest: document\n"
         "Sec-Fetch-Mode: navigate\n"
         "Sec-Fetch-Site: none\n"
         "Sec-Fetch-User: ?1\n"
         "TE: Trailers\n"},

        // Firefox98
        {"firefox98",
         "User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64; rv:98.0) Gecko/20100101 Firefox/98.0\n"
         "Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/avif,image/webp,*/*;q=0.8\n"
         "Accept-Language: en-US,en;q=0.5\n"
         "Accept-Encoding: gzip, deflate, br\n"
         "Upgrade-Insecure-Requests: 1\n"
         "Sec-Fetch-Dest: document\n"
         "Sec-Fetch-Mode: navigate\n"
         "Sec-Fetch-Site: none\n"
         "Sec-Fetch-User: ?1\n"
         "TE: Trailers\n"},

    };








private:


};

#endif // BROWSER_H