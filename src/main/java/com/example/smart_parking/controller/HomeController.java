package com.example.smart_parking.controller;

import org.springframework.stereotype.Controller;
import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.RequestMapping;

@Controller
public class HomeController {
    @GetMapping("/")
    public String viewRoot() {
        return "redirect:/home/";
    }

    @GetMapping("/home/")
    public String viewHomepageGuest() {
        return "homePage";
    }

    @GetMapping("/home/admin")
    public String viewHomepageAdmin() {
        return "homePage";
    }
}
