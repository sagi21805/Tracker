use rect::Rect;
mod rect;

#[repr(C)]
pub struct BoundingBox {
    pub rect: Rect,
    pub kind :u16, 
    pub confidance: f32
}

impl BoundingBox {

    pub fn isIntersecting(&self, boundingBoxes: &[BoundingBox], intersetionPercentage: f32) -> bool {
        for boundingBox in boundingBoxes{
            let p = self.rect.intersectionOfUnionPercentage(&boundingBox.rect);
            println!("PERCENTAGE: {}", p);
            if p > intersetionPercentage{
                return true;
            }
        }
        return false;
    }

}

#[no_mangle]
pub extern "C" fn duplicatesCount(boundingBoxes: *mut BoundingBox, size: usize, confidanceMargin: f32, intersetionPercentage: f32) -> usize{
    let boundingBoxes: &mut [BoundingBox] = unsafe { &mut std::slice::from_raw_parts_mut(boundingBoxes, size) };
    //The array is already sorted by confidances, so the first place is the heighest confidance and the last is the lowest confidance.
    
    let mut numOfDups: usize = 0;
    let lazyConfAvg: f32 = (boundingBoxes[0].confidance + boundingBoxes[size-1].confidance) / 2.0;  
    for i in (0..size).rev() {
        let currentBox: &BoundingBox = &boundingBoxes[i];
        if currentBox.confidance < (lazyConfAvg - confidanceMargin) && currentBox.isIntersecting(boundingBoxes, intersetionPercentage){
            numOfDups += 1;
            println!("RECOGNIZED DUPLICATE!");
        } else {
            break;
        }
    }

    return numOfDups
}
