pub struct PointI32 {
    pub x: i32,
    pub y: i32
}

#[repr(C)]
pub struct PointU16 {
    pub x: u16,
    pub y: u16
}

#[repr(C)]
pub struct Rect{
    pub tl: PointU16,
    pub width: u16, 
    pub heigth: u16,
    pub center: PointI32
}

#[no_mangle]
pub extern "C" fn printRects(vectStart: *mut Rect, size: usize){
    let slice: &mut [Rect] = unsafe { std::slice::from_raw_parts_mut(vectStart, size) };

    // for item in slice {
    //     println!("RECT: ");
    //     print!("x: {} y: {} w: {} h: {}, center: [{}, {}]", item.tl.x, item.tl.y, item.width, item.heigth, item.center.x, item.center.y);
    // }
}