#[repr(C)]
pub struct PointI32 {
    pub x: i32,
    pub y: i32
}

#[repr(C)]
pub struct Rect{
    pub x : u16,
    pub y: u16,
    pub width: u16, 
    pub height: u16,
    pub center: PointI32
}

impl Rect {

    pub fn intersectingRect(&self, other: &Rect) -> Rect {

        let x: u16 = self.x.max(other.x);
        let y: u16 = self.y.max(other.y);
        let width: u16 = (self.x + self.width).min(other.x + other.width) - x;
        let height: u16 = (self.y + self.height).min(other.y + other.height) - y;  
        let center: PointI32 = PointI32 { x: (x + (width / 2)) as i32, y: (y + (height / 2)) as i32 }; 

        Rect { x, y, width, height, center } 

    }

    pub fn area(&self) -> u32{
        self.height as u32 * self.width as u32
    }

    pub fn intersectionOfUnionPercentage(&self, other: &Rect ) -> f32{
        let intersectingRectArea:u32 = self.intersectingRect(other).area();
        let unionArea: u32 = self.area() + other.area() - intersectingRectArea;
        println!("Intersecting Area: {}", intersectingRectArea);
        println!("Union Area: {}", unionArea);
        intersectingRectArea as f32 / unionArea as f32

    }

}
