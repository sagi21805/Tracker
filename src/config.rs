use serde::{Deserialize, Serialize};
use std::fs::File;
use std::io::Read;

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
        let mut file = File::open(file_path).expect("Can't find configuration file");
        let mut data = String::new();
        file.read_to_string(&mut data)?;
        let config: Config = serde_json::from_str(&data)?;
        Ok(config)
    }
}
