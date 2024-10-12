use serde::{Deserialize, Serialize};
use std::fs::File;
use std::io::Read;
use std::path::Path;

use crate::config;

#[derive(Serialize, Deserialize, Debug)]
pub struct Config {
    pub elements_in_point: usize,
    pub min_score: f32,
    pub move_to_last_seen: u32,
    pub min_confidence: f32,
    pub size_coefficient: f32,
    pub vel_coefficient: f32,
    pub ratio: f32,
}

impl Config {
    // Load config from a JSON file
    pub fn from_json_file(file_path: &str) -> Result<Self, Box<dyn std::error::Error>> {
        if Path::new(file_path).exists() {
            let mut file = File::open(file_path).expect("Can't find configuration file");
            let mut data = String::new();
            file.read_to_string(&mut data)?;
            let config: Config = serde_json::from_str(&data)?;
            Ok(config)
        } else {
            println!("Configuration file not found, loading default configuration");
            println!("Configuration is {:?}", Config::default());
            std::thread::sleep(std::time::Duration::from_secs(2));
            Ok(Config::default())
        }
    }
}

impl Default for Config {
    fn default() -> Self {
        Config {
            elements_in_point: 4,
            min_score: 0.5,
            move_to_last_seen: 25,
            min_confidence: 0.0,
            size_coefficient: 0.75,
            vel_coefficient: 2.0,
            ratio: 0.1,
        }
    }
}
